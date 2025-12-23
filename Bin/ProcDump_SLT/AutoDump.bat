@echo on
md D:\CrashDumps
set APP_NAME=SLT_MC_D.exe
set FULL=-ma
set MINI=-mm
set DUMP_OPT=%FULL%
procdump.exe -t -ma %APP_NAME% D:\CrashDumps >> D:\CrashDumps\CrashDump.log