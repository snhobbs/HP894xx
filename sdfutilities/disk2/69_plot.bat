@echo off

REM	69_PLOT.BAT [port] [baud]

REM	Plot all HP 3569A data files that are transferred over the RS-232
REM	port.  The default port is '1' (COM1). The default baud rate is '9600'.

set port69=%1
set baud69=%2
set temp69=temp69
set dire69=tempsdf

if '%port69%'=='' set port69=1
if '%baud69%'=='' set baud69=9600

mkdir %dire69%
chdir %dire69%
echo y | del %dire69%

download /o %temp69%.x32 /p%port69% /b%baud69%

x32tobin /o %temp69%.x32 %temp69%.bin
del %temp69%.x32

69tosdf  /o %temp69%.bin
del %temp69%.bin

for %%f in (*.dat) do viewdata %%f /t:%%f /p >nul
chdir ..

echo y | del %dire69%
rmdir %dire69%

set port69=
set baud69=
set temp69=
set dire69=
