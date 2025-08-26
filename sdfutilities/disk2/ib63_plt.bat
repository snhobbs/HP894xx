@echo off

REM    IB63_PLT.BAT

REM    Upload HP 3563A/62A active trace via HP-IB and plot it.

set file63=HP3563

hpib63 %file63%.63 /o

viewdata %file63%.63 /p
del %file63%.63

set file63=
