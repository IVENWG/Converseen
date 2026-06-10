@echo off
setlocal EnableExtensions
cd /d E:\lessmb

call package\Windows\build_lessmb.bat
if errorlevel 1 exit /b 1

call package\Windows\deploy_lessmb.bat
if errorlevel 1 exit /b 1

call package\Windows\test_and_package.bat
if errorlevel 1 exit /b 1

echo RELEASE_ALL_OK
exit /b 0