@echo off

REM    63_SDF.BAT [drive:]

REM    Convert all HP 3563A/62A data files on the specified LIF disk drive
REM    to SDF.  Default drive is 'b:'.

set drv63=%1
set dir63=tmp63
set file63=*.DAT
set tmp63=%dir63%\%file63%

if '%drv63%'=='' set drv63=b:
mkdir %dir63%
if exist %tmp63% del %tmp63%

echo Copy HP 3563A/62A LIF data files from %drv63%
lif cp %drv63%DA* %tmp63% /o

chdir %dir63%
echo Convert HP 3563A/62A data files to SDF
for %%f in (%file63%) do 63tosdf %%f ..\%%f /o
chdir ..

del %tmp63%
rmdir %dir63%

set drv63=
set dir63=
set file63=
set tmp63=
