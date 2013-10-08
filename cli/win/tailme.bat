@echo off
rem ****** MAIN ****** 
IF "%1"=="-d" GOTO displayfile
IF "%1"=="-f" GOTO followfile

:help
echo usage:
echo 1. display file
echo tail.bat -d nlines file
echo 2. tail file
echo tail.bat -f file

GOTO end

rem ************ 
rem Show Last n lines of file
rem ************ 

:displayfile
SET skiplines=%2
SET sourcefile=%3

rem *** Get the current line count of file ***
FOR /F "usebackq tokens=3 delims= " %%l IN (`find /c /v "" %sourcefile%`) DO (call SET find_lc=%%l)

rem *** Calculate the lines to skip
SET /A skiplines=%find_lc%-!skiplines!

rem *** Display to screen line needed
more +%skiplines% %sourcefile%

GOTO end

rem ************ 
rem Show Last n lines of file & follow output
rem ************ 

:followfile
SET skiplines=0
SET lc=0
SET sourcefile=%2

FOR /F "usebackq tokens=3 delims= " %%l IN (`find /c /v "" %sourcefile%`) DO (call SET lc=%%l)
rem show last 5 lines at startup
SET /A skiplines=%lc%-5

:followloop
rem *** Get the current line count of file ***
FOR /F "usebackq tokens=3 delims= " %%l IN (`find /c /v "" %sourcefile%`) DO (call SET lc=%%l)


rem *** Display to screen line when file updated
more +%skiplines% %sourcefile%

rem *** Calculate the lines to skip
SET /A skiplines=%lc%
rem echo lines = %skiplines% >>skip.txt
rem echo lines = %lc% >>lc.txt

goto followloop

:end