@echo off
rem del /?    or    help del
rem  /f Force deleting of read-only files
rem  /s Delete specified files from all subdirectories.
rem  /q Quite mode, do not ask if ok to delete on global wildcard
echo remove %1
del /f /s /q %1\*
rem rd /?
rem  /s Remove all directories and files in the specified directory in addition to the directory itself. Used to remove a directory tree.
rem  /q Quite mode, do not ask if ok to remove a directory tree with /s
rem rd /s /q %1
for /f "delims=" %%x in ('dir /b /ad %1') do rd /s /q "%%x"
