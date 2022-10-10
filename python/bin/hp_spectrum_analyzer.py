#!/usr/bin/env python
import socket
from time import sleep
import datetime
import plx_gpib_ethernet
import click

def read_data(gpib, arg):
    data = []
    gpib.write(arg)
    for i in range(1):
        while True:
            try:
                data.extend(gpib.read())
                sleep(0.1)
            except Exception as e:
                print(e, i)
                break
    d = "".join(data).strip().split(",")
    return [float(pt) for pt in d if len(pt.strip())]


def make_name(name):
    time_stamp = datetime.datetime.now().isoformat().replace(".", ":")
    return "{}_{}".format(name, time_stamp)


def plot(name):
    from matplotlib import pyplot as plt
    with open(name + ".dat", "r") as f:
        d = f.read().strip().split("\n")
        plt.semilogy([float(pt) for pt in d if len(pt.strip()) and "#" not in pt])
    plt.show()


def data_run(gpib):
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
        "CALC:X:UNIT:POWER?",
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

    header = []

    for query in queries:
        sleep(0.1)
        try:
            value = gpib.query(query).strip()
            print(query, value)
            header.append((query, value))
        except socket.timeout:
            pass

    data = read_data(gpib, "CALC:DATA?")

    # duration = 0.5
    # freq = 2000
    # os.system('play --no-show-progress --null --channels 1 synth %s sine %f' % (duration, freq))
    return header, data


@click.option('--name', required=True, type=str, help='file name')
@click.option('--ip', default="10.231.231.129", type=str, help='IP address')
@click.option('--timeout', "-t", default=3, type=int, help='Timeout')
#@click.option('--port', "-p", default=1234, type=int, help='IP address')
@click.option('--gpib-address', "-g", default=19, type=int, help='GPIB address')
@click.command()
def take_data(name, ip, timeout, gpib_address):
    gpib = plx_gpib_ethernet.PrologixGPIBEthernet(ip, timeout)
    gpib.connect()
    gpib.select(gpib_address)
    header, data = data_run(gpib)


    name = make_name(name)
    header = [("file_name", name),
        ("date", datetime.datetime.now().date()),
        ("time", datetime.datetime.now().time())] + header

    with open(name + ".dat", 'w') as f:
        for key, value in header:
            f.write("# {}: {}\n".format(key, value))
        f.write("\n".join([str(pt) for pt in data]))
    plot(name)
    print(name)
    gpib.close()
    return None



if __name__ == "__main__":
    take_data()
