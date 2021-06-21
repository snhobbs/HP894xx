import click, logging, sys
@click.group()
@click.option('--config', multiple = True, type=str, required=True, help='Config files, multiple accepted')
@click.pass_context
def gr1(ctx, config):
    ctx.ensure_object(dict)
    ctx.obj['config'] = LabAuto.ParseConfigFiles(config)

@gr1.command()
@click.option('--command_file', '-c', type=str, default='', help='GPIB Command List')
@click.option('--test', '-t', is_flag=True, help='GPIB Command List')
@click.pass_context
def LoadSetup(ctx, command_file, test):
    commands = LabAuto.ParseHPCommandFile(command_file)

    config = ctx.obj['config']
    with HP89400Device(int(config['gpib']['port']), config['tcpip']['hostname'], config['tcpip']['port']) as hp_device:
        setup_commands = [pt[1] for pt in config['prologix_commands'].items()]

        _log.info(hp_device.idn())
        hp_device.write("*CLS")
        _log.info(hp_device.query("FORM?"))
        _log.info(hp_device.query("FORM:DATA?"))

        _log.info("#"*50)
        _log.info("\t\tSetup Device")
        _log.info("#"*50)
        #hp_device.Setup(setup_commands)
        hp_device.RunCommands(setup_commands)

        '''
        bridge.Write("INP1:IMP 50\r\n")
        bridge.Write("INP1:IMP 50\r\n")
        bridge.Write("SENS:AVER:STAT OFF\r\n")
        #bridge.Write("SENS:AVER:TYPE COMP\r\n")
        bridge.Write("SENS:BAND:MODE:ARB on\r\n")
        bridge.Write("SENS:BAND:RES 100 khz\r\n")
        bridge.Write("SENS:BAND:AUTO off\r\n")
        bridge.Write("SENS:BAND:RES:AUTO:OFFS off\r\n")
        #bridge.Write("SENS:DDEM:ADAP off\r\n")
        bridge.Write("SENS:FREQ:BAS on\r\n")
        bridge.Write("SENS:FREQ:CENT 1 mhz\r\n")
        bridge.Write("SENS:FREQ:MAN 1 mhz\r\n")
        bridge.Write("SENS:FREQ:SPAN 1 mhz\r\n")
        #bridge.Write("SENS:FREQ:SPAN:FULL\r\n")
        bridge.Write("SENS:FREQ:STAR 1 mhz\r\n")
        bridge.Write("SENS:FREQ:STOP 2 mhz\r\n")
        bridge.Write("SENS:FREQ:STEP:AUTO off\r\n")
        bridge.Write("SYST:ERR?\r\n")
        print(bridge.Read(mode = "eoi"))
        '''
@gr1.command()
@click.pass_context
@click.option('--filename', '-f', type=str, default='gpib_data.tiff', help='filename to save image to')
def SaveTiff(ctx, filename):
    config = ctx.obj['config']
    with HP89400Device(int(config['gpib']['port']), config['tcpip']['hostname'], config['tcpip']['port']) as hp_device:
        tiff = hp_device.read_tiff()
        with open(filename, "+wb") as f:
            f.write(tiff)

@gr1.command()
@click.pass_context
@click.option('--filename', '-f', type=str, default='gpib_data.csv', help='filename to save data to')
def SaveData(ctx, filename):
    config = ctx.obj['config']
    with HP89400Device(int(config['gpib']['port']), config['tcpip']['hostname'], config['tcpip']['port']) as hp_device:
        data = hp_device.read_data()
        with open(filename, "+wb") as f:
            f.write(data)

def Run():
    global _log
    #logging.basicConfig()
    _log = logging.getLogger()
    _log.setLevel(logging.DEBUG)
    _log.addHandler(logging.StreamHandler(sys.stdout))
    assert(_log == logging.getLogger())
    try:
        gr1()
    except Exception as e:
        raise

if __name__ == "__main__":
    Run()

