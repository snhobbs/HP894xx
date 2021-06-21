import logging
from time import sleep
import numpy as np
from timeout_decorator import timeout
import timeout_decorator

"""
HP GPIB Device
-> 2 types of commands:
    1) Ethernet-GPIB bridge meta commands
    2) GPIB control commands
Bridge commands have different delimiters for lines, newlines and returns might be stripped? FIXME
GPIB commands that are passed through have a configurable line delimiter added by the bridge


"""

'''
File Format is line number !${command}
The first 3 lines are meta data
'''
MEASUREMENT_FINISH_SLEEP_TIME = 0
DATA_READ_SLEEP_TIME = 0
#MEASUREMENT_FINISH_SLEEP_TIME = 0.05
#DATA_READ_SLEEP_TIME = 0.1

def ParseHPCommandFile(fname):
    kMetaDataRows = 3
    lines = np.loadtxt(fname, skiprows=kMetaDataRows, delimiter="!", dtype=str, unpack=True)
    logging.getLogger().info(lines)
    return lines[1]

def ReadStatusCode(response):
    resp_str = response.decode("utf-8")
    resp_code = int(resp_str.split(',')[0])
    log = logging.getLogger()
    #log.info("Status Code %d, %s"%(resp_code, resp_str))
    return resp_code

class HPDevice:
    def __init__(self, bridge, gpib_address):
        self.bridge = bridge
        self.bridge.open()
        logging.getLogger().info("Connection Successful")
        self.SetupBridgeCommunication(gpib_address)

    def Read(self):
        return self.bridge.AsciiRead()

    def Write(self, arg):
        return self.bridge.Write("%s\n"%arg.strip())

    def RunCommands(self, commands):
        for line, command in enumerate(commands):
            command = command.strip()
            logging.getLogger().info("%d: %s"%(line, command))
            if "?" in command:
                self.SendQuery(command)
            else:
                self.SendCommand(command)

    def SetupBridgeCommunication(self, gpib_address: int):
        self.bridge.Flush()
        self.bridge.SetAddress(gpib_address)
        # read until eoi is recieved
        # no eoi signal at last character
        self.SendCommand("++eoi 1")
        # append to end of command, 0 = CR+LF, 1=CR, 2=LF, 3=None
        self.SendCommand("++eos 0")
        # no user defined character after transmit
        self.SendCommand("++eot_enable 1")
        # not used
        self.SendCommand("++eot_char %d"%ord("\n"))
        #eot_char = ++eot_char 42
        # disable front panel during operation
        #llo = ++llo
        # enable front panel
        #loc = ++loc
        # timeout in ms, only used when using timeout read
        self.SendCommand("++read_tmo_ms 1000")

    @timeout(2)
    def SendCommand(self, command):
        """
        A ++read after a command (nonquery) leads to an "unterminated response"
        error
        """
        log = logging.getLogger()
        #log.info(command)
        assert("?" not in command)
        while True:
            # self.ClearDataBuffer()
            self.Write(command)
            status_response = self.ReadState()
            #log.info("%s\tResponse: %s"%(command, status_response))
            try:
                if ReadStatusCode(status_response) == 0:
                    break
            except (TypeError, ValueError) as error:
                log.warning("Gpib Send Command Error {}".format(error))
                self.ClearErrors()
    @timeout(2)
    def SendQuery(self, command):
        log = logging.getLogger()
        #log.info(command)
        # assert("?" in command)
        while True:
            self.ClearDataBuffer()
            self.Write(command)
            response = self.Read()
            status_response = self.ReadState()
            #log.info("%s\t%s\tstatus: %s"%(command, response, status_response))
            try:
                if ReadStatusCode(status_response) == 0:
                    break
            except (TypeError, ValueError) as error:
                log.warning(error)
                self.ClearErrors()
        return response

    @timeout(2)
    def ClearDataBuffer(self):
        self.bridge.Flush()

    @timeout(5)
    def ReadState(self):
        while True:
            # self.ClearDataBuffer()
            self.Write("syst:err?")
            response = self.Read()
            if response is not None:
                break
        return response

    @timeout(5)
    def ClearErrors(self):
        no_errors = False
        while not no_errors:
            try:
                resp = self.ReadState()
                code = ReadStatusCode(resp)
                if code == 0:
                    no_errors = True
            except (ValueError, TimeoutError, timeout_decorator.TimeoutError):
                pass

    def __del__(self):
        self.bridge.close()

class HP89400(HPDevice):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.xunits = None
        self.yunits = None
        self.freq_center = None
        self.freq_span = None

    @timeout(30)
    def AutoRangeBlocking(self):
        self.SendCommand("SENS:VOLT:RANGE:AUTO ONCE")
        logging.getLogger().info("Auto Range")
        while True:
            resp = self.SendQuery("STAT:OPER:COND?")
            logging.getLogger().info("Status {} {}".format(self.ReadState(), resp))
            try:
                if (int(resp) & (1<<2)) == 0:
                    break
            except (ValueError, TypeError) as error:
                logging.getLogger().warning(error)

    #def Setup(self):
    #    super().Setup()
    #    self.SendCommand("*CLS")
    #    self.SendCommand("SENS:VOLT:RANGE:AUTO 0")
    #    #gpib.Write("SENS:VOLT:RANGE:AUTO ONCE")
    #    self.SendCommand("cont")

    def SetupMeasurement(self):
        self.SendCommand("SENS:VOLT:RANGE:AUTO 0")
        self.AutoRangeBlocking()
        logging.getLogger().info(self.ReadState())
        self.SendCommand("abor")

    def GetReadDataPoints(self):
        return self.SendQuery("SENS:AVER:COUN:INT?")

    @timeout(30)
    def WaitForMeasurementToFinish(self, kAverageDataPoints):
        while True:
            sleep(MEASUREMENT_FINISH_SLEEP_TIME)
            counts = self.GetReadDataPoints()
            logging.getLogger().info(counts)
            try:
                if int(counts) >= kAverageDataPoints:
                    break
            except TypeError:
                pass

    @timeout(30)
    def ReadAnalyzerData(self):
        self.StartDataRead()
        sleep(DATA_READ_SLEEP_TIME)
        data = bytearray()
        data.extend(self.bridge.ReadBlob())
        return data

    def StartDataRead(self):
        self.Write("CALC:DATA?")

    def ReadConfiguration(self):
        self.xunits = self.GetXUnits()
        self.yunits = self.GetYUnits()
        self.freq_center = self.GetXCenter()
        self.freq_span = self.GetXSpan()
        #self.data_length = self.GetDataLength()

    def GetDataLength(self):
        return int(self.SendQuery("CALC:DATA:HEAD:POIN?"))

    def GetYUnits(self):
        #gpib.Write("CALC:UNIT:AM?\r\n")
        return self.SendQuery("CALC:UNIT:POW?")

    def GetXUnits(self):
        return self.SendQuery("CALC:X:UNIT:FREQ?")

    def GetXCenter(self):
        return self.SendQuery("SENS:FREQ:CENT?")

    def GetXSpan(self):
        return self.SendQuery("SENS:FREQ:SPAN?")

class DummyHP89400(object):
    def __init__(self, bridge, gpib_address):
        logging.getLogger().info("Connection Successful")

    def Read(self):
        return 0

    def Write(self, arg):
        return

    def RunCommands(self, commands):
        for line, command in enumerate(commands):
            command = command.strip()
            logging.getLogger().info("%d: %s"%(line, command))

    def SetupBridgeCommunication(self, gpib_address: int):
        pass

    def SendCommand(self, command):
        pass

    def SendQuery(self, command):
        pass

    def ClearDataBuffer(self):
        pass

    def ReadState(self):
        return 0

    def ClearErrors(self):
        pass

    def AutoRangeBlocking(self):
        pass

    def SetupMeasurement(self):
        pass

    def GetReadDataPoints(self):
        return 36823

    def WaitForMeasurementToFinish(self, kAverageDataPoints):
        pass

    def ReadAnalyzerData(self):
        return bytes(0xfa for _ in range(self.GetReadDataPoints()))

    def StartDataRead(self):
        pass

    def GetDataLength(self):
        pass

    def GetYUnits(self):
        return b"yunits"

    def GetXUnits(self):
        return b"xunits"

    def GetXCenter(self):
        return b"5e6"

    def GetXSpan(self):
        return b"10e6"
