@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
set PATH=C:\Qt\6.8.3\msvc2022_64\bin;%PATH%
cd /d E:\lessmb\build
windeployqt6 lessmb.exe --release --no-translations
copy /Y C:\ImageMagick-64\CORE_RL_*.dll .
if not exist modules mkdir modules
xcopy /Y /E /I C:\ImageMagick-64\modules modules
copy /Y C:\ImageMagick-64\*.xml .
copy /Y C:\ImageMagick-64\sRGB.icc .
echo DEPLOY_OK
dir lessmb.exe