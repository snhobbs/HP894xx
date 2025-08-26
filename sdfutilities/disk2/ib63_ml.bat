@echo off

REM    IB63_ML.BAT filename

REM    Upload HP 3563A/62A active trace via HP-IB and convert it to
REM    PC-MATLAB Mat-file format.  Destination is filename.MAT.

set file63=%1
if '%file63%'=='' set file63=HP3563

hpib63 %file63%.63 /o

63tosdf %file63%.63 %file63%.DAT /o
del %file63%.63

sdftoml %file63%.DAT /b /o >nul
del %file63%.DAT

set file63=
