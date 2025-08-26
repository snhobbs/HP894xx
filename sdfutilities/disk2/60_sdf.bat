@echo off

REM	60_SDF.BAT [port] [baud]

REM	Convert all HP 3560A data files that are transferred over the RS-232
REM	port to SDF.  The default port is '1' (COM1).  The default baud rate
REM	is '9600'.

set port60=%1
set baud60=%2
set temp60=temp60

if '%port60%'=='' set port60=1
if '%baud60%'=='' set baud60=9600

download /o %temp60%.x32 /p%port60% /b%baud60%

x32tobin /o %temp60%.x32 %temp60%.bin
del %temp60%.x32

60tosdf  /o %temp60%.bin
del %temp60%.bin

set port60=
set baud60=
set temp60=
