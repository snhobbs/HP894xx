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
 # file_name: labamp_dark_5mhz_2022-10-10T12:18:22:421802
 # date: 2022-10-10
 # time: 12:18:22.421875
 # SENS:FREQuency:STEP?: +2.50000000000000E+004
 # FREQuency:STARt?: +0.00000000000000E+000
 # FREQuency:STOP?: +5.00000000000000E+006
 # FREQuency:SPAN?: +5.00000000000000E+006
 # FREQuency:CENTer?: +2.50000000000000E+006
 # SENSe:BANDwidth:RESolution?: +1.00000000000000E+005
 # SENS:AVERAGE:COUNT?: +4000
 # OUTPut:IMPedance?: +5.00000000000000E+001
 # INSTRUMENT:SEL?: SCAL
 # INSTRUMENT:NSEL?: +1
 # INPUT1:COUPLING?: AC
 # INPUT2:COUPLING?: AC
 # CALC:X:UNIT:TIME?: 
 # CALC:X:UNIT:FREQ?: HZ
 # CALC:UPHASE:OFFSET?: +0.00000000000000E+000
 # CALC:UPHASE:CREF?: +0.00000000000000E+000
 # CALC:UNIT:TIME?: 
 # CALC:UNIT:POWER?: VRMS/RTHZ
 # CALC:UNIT:FREQ?: 
 # CALC:UNIT:ANGLE?: 
 # CALC:UNIT:AM?: 
 # CALC:DATA:HEAD:POINTS?: +1601
 # CALC:FORMAT?: MLOG
 5.42512054835242e-07
 5.42191457952867e-07
 5.41927079211746e-07
 5.41535314368957e-07
 5.40725523023866e-07
 5.39124130227719e-07
 5.36303559783846e-07
 5.31816908733163e-07
 5.25229324921384e-07
 5.16566274200159e-07
...
```
The header entries start with a '#' and have a space colon space (" : ") seperating the field name and value. No x values are saved, only y values. The x values can be determined from the header.
