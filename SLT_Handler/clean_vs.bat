rem --------------------------------------------------------
rem microsoft
rem --------------------------------------------------------
rem del /s *.obj
del /s *.pch
del /s *.idb
del /s *.pdb
del /s *.exp
del /s *.ilk
del /s *.ncb
del /s *.aps
del /s *.plg
del /s *.opt
del /s *.bsc
del /s *.sbr
del /s *.VC.db
rem del /s *.user
del /s *.dep
del /s *.manifest
rem del /s *.user
del /s *.vspscc
del /s *_i.c
del /s *.i
del /s *.icf
del /s _p.c
del /s *.exe
del /s *.tlb
del /s *.tlh
del /s *.cache
del /s *.log
del /s *.sdf
rem del /s *.obj
del /s *.manifest.res
del /s BuildLog.htm
del /s /ah *.suo

rem --------------------------------------------------------
rem others
rem --------------------------------------------------------
del /s *.bak

rem --------------------------------------------------------
rem folder
rem --------------------------------------------------------
rmdir /s/q log
rmdir /s/q Debug
rmdir /s/q Release
rmdir /s/q __history
rmdir /s/q ipch
rmdir /s/q .vs

@echo off
FOR /F "tokens=*" %%G IN ('DIR /B /AD /S Debug') DO RMDIR /S /Q "%%G"
FOR /F "tokens=*" %%G IN ('DIR /B /AD /S Release') DO RMDIR /S /Q "%%G"
FOR /F "tokens=*" %%G IN ('DIR /B /AD /S ipch') DO RMDIR /S /Q "%%G"

rem dir /AD /S /B Debug > del.txt
rem FOR /F %a IN (del.txt) DO rmdir /S /Q %a

rem dir /AD /S /B ipch > del.txt
rem FOR /F %a IN (del.txt) DO rmdir /S /Q %a

rem dir /AD /S /B release > del.txt
rem FOR /F %a IN (del.txt) DO rmdir /S /Q %a
rem del del.txt

pause