@echo off
setlocal EnableExtensions

call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b 1

set PATH=C:\Tools\cmake\cmake-3.31.6-windows-x86_64\bin;C:\Qt\6.8.3\msvc2022_64\bin;%PATH%
set Qt6_DIR=C:\Qt\6.8.3\msvc2022_64\lib\cmake\Qt6

cd /d E:\lessmb

if exist package\Windows\ghostscript\bin\gswin64c.exe (
  echo === Ghostscript bundle already present ===
) else (
  echo === Bundle Ghostscript ===
  powershell -NoProfile -ExecutionPolicy Bypass -File package\Windows\bundle-ghostscript.ps1
  if errorlevel 1 exit /b 1
)

if exist build rmdir /s /q build
mkdir build
cd build

cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release -DUSE_QT6=yes -DImageMagick_EXECUTABLE_DIR=C:\ImageMagick-64 ..
if errorlevel 1 exit /b 1

nmake
if errorlevel 1 exit /b 1

if not exist loc mkdir loc
lrelease ..\loc\converseen_zh_CN.ts -qm loc\converseen_zh_CN.qm
if errorlevel 1 exit /b 1

echo BUILD_OK
dir lessmb.exe
exit /b 0