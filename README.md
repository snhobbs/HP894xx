# HP894xx
HP89400 Series Vector Signal Analyzer GPIB Control using the Prologix GPIB-ETHERNET adapter.

## Basic Usage
+ The hp_spectrum_analyzer.py script is all that is needed for simple data taking
```{sh}
hp_spectrum_analyzer.py --name name_of_data_run --gpib-address 19 --ip=10.231.231.128
```
## Script Procedure
+ Pulls all available metadata into the header. Some different machines have different options so not all requests will succeed. The script will catch the error and continue.
+ Pulls data from buffer.
+ Saves file with timestamp to ensure unique file names. This ensures that data isn't overwritten.

## Data Format
+ Output files look like this:
```
 # file_name: labamp_10khz_10mhz_swept_sine
 # date: 2022-10-10
 # time: 12:58:26.045481
 # BYT_NR : 2
 # BIT_NR : 16
 # ENCDG : ASC
 # BN_FMT : RP
 # BYT_OR : MSB
 # CH2:WFID : "Ch2, DC coupling, 5.000 Volts/div, 50.00ms/div, 15000 points, Pk Detect mode"
 # NR_PT : 15000
 # PT_FMT : Y
 # XUNIT : "s"
 # XINCR : 1.000E-3
 # XZERO : 0.0E+0
 # PT_OFF : 13500
 # YUNIT : "Volts"
 # YMULT : 31.25E-6
 # YOFF : 128.0E+0
 # YZERO : 0.0E+0
 # :WFMPRE:CH3:WFID : "Ch3, DC coupling, 200.0mVolts/div, 50.00ms/div, 15000 points, Pk Detect mode"
 -16128.0
 -15360.0
 -16128.0
 -15360.0
 -16128.0
 -15360.0
 -16128.0
 -15360.0
 -16128.0
 -15360.0
...
```
The header entries start with a '#' and have a space colon space (" : ") seperating the field name and value. No x values are saved, only y values. The x values can be determined from the header.
