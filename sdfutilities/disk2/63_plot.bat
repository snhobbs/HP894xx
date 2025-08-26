@echo off

REM    63_PLOT.BAT [drive:]

REM    Plot all HP 3563A/62A data files on the specified LIF disk drive.
REM    Default drive is 'b:'.

set drv63=%1
set dir63=tmp63
set file63=*.63
set tmp63=%dir63%\%file63%

if '%drv63%'=='' set drv63=b:
mkdir %dir63%
if exist %tmp63% del %tmp63%

echo Copy HP 3563A/62A LIF data files from %drv63%
lif cp %drv63%DA* %tmp63% /o

REM plot all *.63 files
chdir %dir63%
for %%f in (%file63%) do viewdata %%f /t:%%f /p
chdir ..

del %tmp63%
rmdir %dir63%

set drv63=
set dir63=
set file63=
set tmp63=
