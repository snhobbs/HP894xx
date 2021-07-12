import logging
from time import sleep
import numpy as np
from timeout_decorator import timeout
import timeout_decorator
import socket
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

'''
HP GPIB Device
-> 2 types of commands:
    1) Ethernet-GPIB bridge meta commands
    2) GPIB control commands
Bridge commands have different delimiters for lines, newlines and returns might be stripped? FIXME
GPIB commands that are passed through have a configurable line delimiter added by the bridge
'''
import time
class HP89400Device(PrologixGPIBEthernetDevice):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.xunits = None
        self.yunits = None
        self.freq_center = None
        self.freq_span = None

    def __enter__(self):
        print("Open Socket", time.time())
        self.connect()
        print("Socket Opened", time.time())
        return self

    def __exit__(self, *args, **kwargs):
        print("Close Socket", time.time())
        self.close()
        print("Socket Closed ", time.time())

    @classmethod
    def get_error_code(cls, error):
        return int(str(error[:3]).strip(','))

    def reset(self):
        self.close()
        self.gpib = PrologixGPIBEthernet(self.gpib.host, self.gpib.timeout)
        self.connect()

    def write(self, *args, **kwargs):
        print(*args, **kwargs)
        super().write(*args, **kwargs)
        #error = self.read_errors()
        #if(self.get_error_code(error) != 0):
        #    logging.getLogger().info(error)
        #    assert(self.get_error_code(error) == 0)
        #return resp
    def query(self, *args, **kwargs):
        print(*args, **kwargs)
        return super().query(*args, **kwargs)

    def read_error_code(self):
        return self.get_error_code(self.read_errors())

    def read_errors(self):
        return self.query("SYST:ERR?\r\n").strip()

    def read_data(self):
        return self.query("CALC:DATA?\r\n")
        #self.write("CALC:DATA?\r\n")
        #return self.read()

    def read_tiff(self):
        self.write("HCOP:DEV:LANG tiff")
        blob = self.query("HCOP:DATA?\r\n")
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
        #super().write("abor")
        while True:
            try:
                super().write("abor")
                break
            except socket.timeout:
                continue

    def setup(self):
        self.clear_errors()
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

    def get_operating_condition(self):
        resp = int(self.query("STAT:OPER:COND?"))
        return resp

    @timeout(30)
    def AutoRangeBlocking(self):
        self.write("SENS:VOLT:RANGE:AUTO ONCE")
        logging.getLogger().info("Auto Range")
        sleep(1)
        while True:
            resp = self.get_operating_condition()
            logging.getLogger().info("Status {} {}".format(self.read_errors(), resp))
            try:
                if (int(resp) & (1<<2)) == 0:
                    break
            except (ValueError, TypeError) as error:
                logging.getLogger().warning(error)

    def enable_autorange(self, setting):
        self.write("SENS:VOLT:RANGE:AUTO {}".format(int(setting)))

    def GetReadDataPoints(self):
        return int(self.query("SENS:AVER:COUN:INT?"))

    def StartDataRead(self):
        self.write("CALC:DATA?")

    def read_data(self):
        data = []
        self.StartDataRead()
        for i in range(1):
            while True:
                try:
                    data.extend(self.read())
                    sleep(0.1)
                except Exception as e:
                    print(e, i)
                    break
        delim=','
        d = "".join(data).strip().strip(delim).split(delim)
        return [float(pt) for pt in d if len(pt.strip())]

    def ReadConfiguration(self):
        self.xunits = self.GetXUnits()
        self.yunits = self.GetYUnits()
        self.freq_center = self.GetXCenter()
        self.freq_span = self.GetXSpan()
        #self.data_length = self.GetDataLength()

'''
def data_run(ip, name):
    name = make_name(name)
    queries = [
        "SENS:FREQuency:STEP?",
        "FREQuency:STARt?",
        "FREQuency:STOP?",
        "FREQuency:SPAN?",
        "FREQuency:CENTer?",
        "SENSe:BANDwidth:RESolution?",
        "SENS:AVERAGE:COUNT?",
        "OUTPut:IMPedance?",
        "INSTRUMENT:SEL?",
        "INSTRUMENT:NSEL?",
        "INPUT1:COUPLING?",
        "INPUT2:COUPLING?",
        "CALC:X:UNIT:TIME?",
        #"CALC:X:UNIT:POWER?",
        "CALC:X:UNIT:FREQ?",
        "CALC:X:UNIT:CODE?",
        "CALC:UPHASE:OFFSET?",
        "CALC:UPHASE:CREF?",
        "CALC:UNIT:TIME?",
        "CALC:UNIT:POWER?",
        "CALC:UNIT:FREQ?",
        "CALC:UNIT:ANGLE?",
        "CALC:UNIT:AM?",
        "CALC:DATA:HEAD:POINTS?",
        "CALC:FORMAT?"
    ]
    gpib = setup_gpib(ip)
    #os.system('play --no-show-progress --null --channels 1 synth %s sine %f' % (duration, freq))

    resps = []
    for query in queries:
        print(query)
        sleep(0.1)
        try:
            value = gpib.query(query).strip()
        except Exception as e:
            print(e)
            raise
        resps.append(value)
    delim=','
    data = gpib.read_data().strip().strip(delim).split(delim)

    with open(name + ".dat", 'w') as f:
        f.write(f"# file_name: {name}\n")
        f.write(f"# date: {datetime.datetime.now().date()}\n")
        f.write(f"# time: {datetime.datetime.now().time()}\n")
        for query, value in zip(queries, resps):
            f.write(f"# {query}: {value}\n")
        f.write("\n".join([str(pt) for pt in data]))
    plot(name)
    print(name)
    gpib.close()
    return None



'''
