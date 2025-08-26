@echo off

REM    63_ML.BAT [drive:]

REM    Convert all HP 3563A/62A data files on the specified LIF disk drive to
REM    PC-MATLAB Mat-file format.  Destination is *.MAT where '*' is the
REM    same name as the LIF file on the source disk drive.
REM    Default drive is 'b:'.

set drv63=%1
set dir63=tmp63
set dirsdf=tmpsdf
set file63=*.DAT
set tmp63=%dir63%\%file63%
set tmpsdf=%dirsdf%\%file63%
set tmpmat=%dirsdf%\*.MAT

if '%drv63%'=='' set drv63=b:
mkdir %dir63%
if exist %tmp63% del %tmp63%
mkdir %dirsdf%
if exist %tmpsdf63% del %tmpsdf%

echo Copy HP 3563A/62A LIF data files from %drv63%
lif cp %drv63%DA* %tmp63% /o

echo Convert HP 3563A/62A data files to SDF
chdir %dir63%
for %%f in (%file63%) do 63tosdf %%f ..\%dirsdf%\%%f /o
chdir ..

echo Convert SDF files to PC-MATLAB Mat-files
chdir %dirsdf%
for %%f in (%file63%) do sdftoml %%f /b /o >nul
echo Copy PC-MATLAB Mat-files to current directory
copy *.MAT ..
chdir ..

del %tmpsdf%
del %tmpmat%
rmdir %dirsdf%
del %tmp63%
rmdir %dir63%

set drv63=
set dir63=
set dirsdf=
set tmp63=
set tmpsdf=
set tmpmat=
