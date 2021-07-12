from time import sleep
import datetime
import click
import os
from HP894xx import HP89400Device
#duration = 0.5
#freq = 2000


def make_name(name):
    time_stamp = datetime.datetime.now().isoformat().replace(".", ":")
    return f"{name}_{time_stamp}"


def plot(name):
    from matplotlib import pyplot as plt
    with open(name + ".dat") as f:
        d = f.read().strip().split("\n")
        plt.semilogy([float(pt) for pt in d if len(pt.strip()) and "#" not in pt])
    plt.show()


def setup_gpib(ip):
    address = 16
    device = HP89400Device(address, ip, timeout=3)
    print(device.address)
    print(device.gpib.host)
    device.connect()
    device.setup();
    return device

def read_configuration(hp_device):
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
    resps = []
    for query in queries:
        print(query)
        sleep(0.1)
        try:
            value = hp_device.query(query).strip()
        except Exception as e:
            print(e)
            raise
        resps.append(value)
    return queries, resps


@click.option('--ip', required=True, type=str, help='ip address')
@click.option('--name', type=str, required=True, help='file name')
@click.command()
def take_data(ip, name):
    hp_device = setup_gpib(ip)
    queries, resps = read_configuration(hp_device)
    data = hp_device.read_data()
    fname = make_name(name)

    with open(fname+".dat", 'w') as f:
        f.write(f"# file_name: {name}\n")
        f.write(f"# date: {datetime.datetime.now().date()}\n")
        f.write(f"# time: {datetime.datetime.now().time()}\n")
        for query, value in zip(queries, resps):
            f.write(f"# {query}: {value}\n")
        f.write("\n".join([str(pt) for pt in data]))
    plot(fname)

if __name__ == "__main__":
    take_data()
