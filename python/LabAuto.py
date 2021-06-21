#!/usr/bin/env python3
import configparser
import click
from datetime import datetime
import os
import logging
import socket
from timeout_decorator import timeout, TimeoutError
from enum import IntEnum
from time import sleep
import numpy as np

'''
class SocketDevice(object):
    def __init__(self, hostname : str, port):
        self.hostname = hostname
        self.port = int(port)
        self.socket = None

    def open(self):
        self.socket = socket.socket(
                family=socket.AddressFamily.AF_INET,
                type = socket.SocketKind.SOCK_STREAM)
        self.socket.connect((self.hostname, self.port))
        if(self.socket is None):
            raise UserWarning("No host {} at {} found".format(self.hostname, self.port))
        self.socket.setblocking(True)

    def close(self):
        self.socket.close()

    def __enter__(self):
        self.open()
        return self

    def __exit__(self, exception_type, exception_value, traceback):
        self.close()

    def Write(self, cmd : str):
        out = bytes(cmd, encoding="utf-8")
        logging.getLogger().info(out)
        self.socket.send(out)

    @timeout(1)
    def GetChar(self):
        return self.socket.recv(1)

    #when setting up a device use spoll, status, and srq
    @timeout(5)
    def ReadLine(self, kMaxLength = 100):
        line = self.socket.recvmsg(kMaxLength)[0]
        logging.getLogger().info(line)
        return line

    def GetResponse(self, kMaxLength = 100):
        resp = None
        try:
            resp = self.ReadLine(kMaxLength)
        except TimeoutError:
            pass
        return resp

    def Flush(self):
        while(True):
            resp = GetResponse()
            if(resp is None):
                break

# FIXME look into other reading methods
class EthernetGPIBBridge(SocketDevice):
    setup_commands = (
        '++ver',
        '++auto 0',#not auto addressing
        '++mode 1',#set controller mode
        '++clr',#clear
        #'++savecfg', #dont save config
        #'++savecfg 0', #dont save config
        '++ifc',#assert control
    )

    def __init__(self, hostname : str, port):
        super().__init__(hostname, port)

    def GetDeviceVersion(self):
        gpib.Write("++ver\n")
        return gpib.GetResponse()

    @timeout(5)
    def ReadBlob(self):
        self.Write("++read eoi\n")
        dout = bytearray()
        try:
            while(True):
                dout.extend(self.GetChar())
        except TimeoutError:
            pass
        return dout[:-1]

    def Read(self, kMaxLength = 100):
        return self.TimeoutRead(kMaxLength)

    def TimeoutRead(self, kMaxLength = 100):
        self.Write("++read\n")
        return self.GetResponse(kMaxLength)

    def EOIRead(self, kMaxLength = 100):
        self.Write("++read\n")
        return self.GetResponse(kMaxLength)

    def AsciiRead(self, kMaxLength = 100):
        #mode is blank for timeout, eoi for eoi, or ascii for a specific character
        self.Write("++read ascii\n")
        return self.GetResponse(kMaxLength)

    def Setup(self):
        for command in self.setup_commands:
            self.Write("%s\n"%command)

    def SetGpibAddress(self, port: int) -> None:
        self.Write("++addr %d\n"%port)
        self.Write("++spoll\n")
'''

'''
File Format is line number !${command}
The first 3 lines are meta data
'''
def ParseHPCommandFile(fname):
    kMetaDataRows = 3
    lines = np.loadtxt(fname, skiprows = kMetaDataRows, delimiter = "!", dtype=str, unpack = True)
    return lines[1]

def MakeCommandTuple(tuple_string : str) -> tuple:
    ret = []
    for entry in tuple_string.strip().strip("()\"\'").split(','):
        ret.append(entry.strip().strip('\"').strip())
    return tuple(ret)

def ParseConfigFiles(files) -> configparser.ConfigParser:
    config = configparser.ConfigParser()
    for f in files:
        if(not os.path.exists(f)):
            raise FileNotFoundError(f)
        config.read(f)
    return config


