@echo off

REM	60_ML.BAT [port] [baud]

REM	Convert all HP 3560A data files that are transferred over the RS-232
REM	port to PC-MATLAB Mat-file format.  The default port is '1' (COM1).
REM	The default baud rate is '9600'.

set port60=%1
set baud60=%2
set temp60=temp60
set dire60=tempsdf

if '%port60%'=='' set port60=1
if '%baud60%'=='' set baud60=9600

mkdir %dire60%
chdir %dire60%
echo y | del %dire60%

download /o %temp60%.x32 /p%port60% /b%baud60%

x32tobin /o %temp60%.x32 %temp60%.bin
del %temp60%.x32

60tosdf  /o %temp60%.bin
del %temp60%.bin

for %%f in (*.dat) do sdftoml %%f /b /o >nul
copy *.mat ..
chdir ..

echo y | del %dire60%
rmdir %dire60%

set port60=
set baud60=
set temp60=
set dire60=
