@echo off
setlocal EnableExtensions
cd /d E:\lessmb\build

echo === CLI smoke test ===
lessmb.exe -p > E:\lessmb\release-test-formats.txt 2>&1
if errorlevel 1 (
  echo FORMAT_TEST_FAILED
  exit /b 1
)
findstr /C:"Total writable" E:\lessmb\release-test-formats.txt >nul
if errorlevel 1 (
  echo FORMAT_OUTPUT_INVALID
  exit /b 1
)
echo FORMAT_TEST_OK

echo === Ghostscript bundled test ===
if not exist E:\lessmb\package\Windows\ghostscript\bin\gswin64c.exe (
  echo GHOSTSCRIPT_BUNDLE_MISSING
  exit /b 1
)
E:\lessmb\package\Windows\ghostscript\bin\gswin64c.exe --version
if errorlevel 1 (
  echo GHOSTSCRIPT_TEST_FAILED
  exit /b 1
)
echo GHOSTSCRIPT_TEST_OK

echo === GUI launch test ===
powershell -NoProfile -Command ^
  "$p = Start-Process -FilePath 'E:\lessmb\build\lessmb.exe' -PassThru; Start-Sleep -Seconds 4; if ($p.HasExited) { Write-Host 'GUI_TEST_FAILED exit=' $p.ExitCode; exit 1 } else { Write-Host 'GUI_TEST_OK pid=' $p.Id; Stop-Process -Id $p.Id -Force; exit 0 }"
if errorlevel 1 exit /b 1

echo === Package release ===
set RELEASE_DIR=E:\lessmb\release\LessMB-1.0.0-win64
set ZIP_FILE=E:\lessmb\release\LessMB-1.0.0-win64.zip
if exist "%RELEASE_DIR%" rmdir /s /q "%RELEASE_DIR%"
if exist "%ZIP_FILE%" del /f /q "%ZIP_FILE%"
mkdir "%RELEASE_DIR%"
mkdir "%RELEASE_DIR%\loc"

copy /Y lessmb.exe "%RELEASE_DIR%\"
copy /Y ..\res\lessmb.ico "%RELEASE_DIR%\"
copy /Y ..\docs\user-guide.html "%RELEASE_DIR%\"
copy /Y *.dll "%RELEASE_DIR%\"
copy /Y *.xml "%RELEASE_DIR%\"
copy /Y sRGB.icc "%RELEASE_DIR%\"
xcopy /Y /E /I modules "%RELEASE_DIR%\modules"
xcopy /Y /E /I generic "%RELEASE_DIR%\generic"
xcopy /Y /E /I iconengines "%RELEASE_DIR%\iconengines"
xcopy /Y /E /I imageformats "%RELEASE_DIR%\imageformats"
xcopy /Y /E /I networkinformation "%RELEASE_DIR%\networkinformation"
xcopy /Y /E /I platforms "%RELEASE_DIR%\platforms"
xcopy /Y /E /I styles "%RELEASE_DIR%\styles"
xcopy /Y /E /I tls "%RELEASE_DIR%\tls"
copy /Y loc\converseen_zh_CN.qm "%RELEASE_DIR%\loc\"
xcopy /Y /E /I ..\package\Windows\ghostscript "%RELEASE_DIR%\ghostscript"
powershell -NoProfile -ExecutionPolicy Bypass -File ..\package\Windows\clean-ghostscript-bundle.ps1 -BundleRoot "%RELEASE_DIR%\ghostscript"
if errorlevel 1 exit /b 1

echo === PDF smoke test ===
powershell -NoProfile -ExecutionPolicy Bypass -File ..\package\Windows\test-pdf-support.ps1 -ReleaseDir "%RELEASE_DIR%"
if errorlevel 1 exit /b 1

powershell -NoProfile -Command "Compress-Archive -Path '%RELEASE_DIR%' -DestinationPath '%ZIP_FILE%' -Force"
if errorlevel 1 exit /b 1

echo PACKAGE_OK
dir "%ZIP_FILE%"
exit /b 0