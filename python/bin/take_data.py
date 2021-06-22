from time import sleep
import datetime
import click
import os
from HP894xx import HP89400Device
duration = 0.5
freq = 2000

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
    return f"{name}_{time_stamp}"


def plot(name):
    from matplotlib import pyplot as plt
    with open(name + ".dat") as f:
        d = f.read().strip().split("\n")
        plt.plot([float(pt) for pt in d if len(pt.strip()) and "#" not in pt])
    plt.show()


def setup_gpib(ip):
    address = 16
    device = HP89400Device(address, ip, timeout=3)
    print(device.address)
    print(device.gpib.host)
    device.connect()
    device.setup();
    return device

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


@click.option('--ip', required=True, type=str, help='ip address')
@click.option('--name', type=str, required=True, help='file name')
@click.command()
def take_data(ip, name):
    data_run(ip, name)
    hp_device.SetupMeasurement()
    data = hp_device.ReadAnalyzerData()


if __name__ == "__main__":
    take_data()
