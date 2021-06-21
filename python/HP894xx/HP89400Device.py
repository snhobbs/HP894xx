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

from time import sleep
from timeout_decorator import timeout, TimeoutError
from plx_gpib_ethernet import PrologixGPIBEthernetDevice
_log = None
test=False

"""
HP GPIB Device
-> 2 types of commands:
    1) Ethernet-GPIB bridge meta commands
    2) GPIB control commands
Bridge commands have different delimiters for lines, newlines and returns might be stripped? FIXME
GPIB commands that are passed through have a configurable line delimiter added by the bridge


"""
class HP89400Device(PrologixGPIBEthernetDevice):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.xunits = None
        self.yunits = None
        self.freq_center = None
        self.freq_span = None

    def __enter__(self):
        self.connect()
        return self

    def __exit__(self, *args, **kwargs):
        self.close()

    def read_errors(self):
        self.write("SYST:ERR?\r\n")
        resp = (self.read())
        return resp

    def read_data(self):
        self.write("CALC:DATA?\r\n")
        return self.read()

    def read_tiff(self):
        self.write("HCOP:DEV:LANG tiff")
        self.write("HCOP:DATA?\r\n")
        blob = self.read()
        return blob[2:]#FIXME bad first 2 bytes

    def GetDataLength(self):
        return int(self.query("CALC:DATA:HEAD:POIN?"))

    def GetYUnits(self):
        #gpib.write("CALC:UNIT:AM?\r\n")
        return self.query("CALC:UNIT:POW?")

    def GetXUnits(self):
        return self.query("CALC:X:UNIT:FREQ?")

    def GetXCenter(self):
        return self.query("SENS:FREQ:CENT?")

    def GetXSpan(self):
        return self.query("SENS:FREQ:SPAN?")


    def clear_errors(self):
        self.write("*CLS")

    def abort(self):
        self.write("abor")

    def setup(self):
        # read until eoi is recieved
        # no eoi signal at last character
        self.write("++eoi 1")
        # append to end of command, 0 = CR+LF, 1=CR, 2=LF, 3=None
        self.write("++eos 0")
        # no user defined character after transmit
        self.write("++eot_enable 1")
        # not used
        self.write("++eot_char %d"%ord("\n"))
        #eot_char = ++eot_char 42
        # disable front panel during operation
        #llo = ++llo
        # enable front panel
        #loc = ++loc
        # timeout in ms, only used when using timeout read
        self.write("++read_tmo_ms 1000")


    def RunCommands(self, commands):
        for line, command in enumerate(commands):
            logging.getLogger().info("%d: %s"%(line, command))
            self.write(command + "\n")
            sleep(0.05)
            if '?' in command:
                _log.info(self.read())

            if(test):
                resp = self.read_errors()
                assert(b'0' in resp[:3])

    @timeout(30)
    def AutoRangeBlocking(self):
        self.write("SENS:VOLT:RANGE:AUTO ONCE")
        logging.getLogger().info("Auto Range")
        while True:
            resp = self.query("STAT:OPER:COND?")
            logging.getLogger().info("Status {} {}".format(self.read_errors(), resp))
            try:
                if (int(resp) & (1<<2)) == 0:
                    break
            except (ValueError, TypeError) as error:
                logging.getLogger().warning(error)

    def SetupMeasurement(self):
        self.write("SENS:VOLT:RANGE:AUTO 0")
        self.AutoRangeBlocking()
        logging.getLogger().info(self.read_errors())
        self.abort()

    def GetReadDataPoints(self):
        return int(self.query("SENS:AVER:COUN:INT?"))

    @timeout(30)
    def WaitForMeasurementToFinish(self, kAverageDataPoints):
        while True:
            sleep(MEASUREMENT_FINISH_SLEEP_TIME)
            counts = self.GetReadDataPoints()
            logging.getLogger().info("Data Points: {}/{}".format(counts, kAverageDataPoints))
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
        #data.extend(self.bridge.ReadBlob())
        ret = self.read()
        print(ret, type(ret))
        data.extend(bytes(ret, "utf-8"))
        return data

    def StartDataRead(self):
        self.write("CALC:DATA?")

    def ReadConfiguration(self):
        self.xunits = self.GetXUnits()
        self.yunits = self.GetYUnits()
        self.freq_center = self.GetXCenter()
        self.freq_span = self.GetXSpan()
        #self.data_length = self.GetDataLength()

    
