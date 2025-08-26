@echo off

REM    IB63_SDF.BAT filename

REM    Upload HP 3563A/62A active trace via HP-IB and convert it to
REM    SDF format.  Destination is filename.DAT.

set file63=%1
if '%file63%'=='' set file63=HP3563

hpib63 %file63%.63 /o

63tosdf %file63%.63 %file63%.DAT /o
del %file63%.63

set file63=
