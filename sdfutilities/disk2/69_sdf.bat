@echo off

REM	69_SDF.BAT [port] [baud]

REM	Convert all HP 3569A data files that are transferred over the RS-232
REM	port to SDF.  The default port is '1' (COM1).  The default baud rate
REM	is '9600'.

set port69=%1
set baud69=%2
set temp69=temp69

if '%port69%'=='' set port69=1
if '%baud69%'=='' set baud69=9600

download /o %temp69%.x32 /p%port69% /b%baud69%

x32tobin /o %temp69%.x32 %temp69%.bin
del %temp69%.x32

69tosdf  /o %temp69%.bin
del %temp69%.bin

set port69=
set baud69=
set temp69=
