import plx_gpib_ethernet
from time import sleep
import datetime
import click
import os
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


def setup_gpib():
    gpib = plx_gpib_ethernet.PrologixGPIBEthernet("10.231.231.129", 3)
    gpib.connect()
    gpib.select(16)
    return gpib

def data_run(name):
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
    gpib = setup_gpib()
    #os.system('play --no-show-progress --null --channels 1 synth %s sine %f' % (duration, freq))

    with open(name + ".dat", 'w') as f:
        f.write(f"# file_name: {name}\n")
        f.write(f"# date: {datetime.datetime.now().date()}\n")
        f.write(f"# time: {datetime.datetime.now().time()}\n")
        for query in queries:
            sleep(0.1)
            print(query)
            value = gpib.query(query).strip()
            f.write(f"# {query}: {value}\n")
        data = read_data(gpib, "CALC:DATA?")
        f.write("\n".join([str(pt) for pt in data]))
    plot(name)
    print(name)
    gpib.close()
    return None


@click.option('--name', type=str, help='file name')
@click.command()
def take_data(name):
    data_run(name)


if __name__ == "__main__":
    take_data()
