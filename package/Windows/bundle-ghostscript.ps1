# Downloads and stages Ghostscript for the LessMB Windows portable bundle.

$ErrorActionPreference = "Stop"

$GsVersion = "10.07.1"
$GsTag = "gs10071"
$GsInstaller = "gs10071w64.exe"
$GsUrl = "https://github.com/ArtifexSoftware/ghostpdl-downloads/releases/download/$GsTag/$GsInstaller"
$SevenZipUrl = "https://www.7-zip.org/a/7zr.exe"
$SevenZipExtraUrl = "https://www.7-zip.org/a/7z2409-extra.7z"

$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$BundleRoot = Join-Path $ScriptDir "ghostscript"
$WorkRoot = Join-Path $ScriptDir "_ghostscript_work"
$InstallerPath = Join-Path $WorkRoot $GsInstaller
$SevenZipPath = Join-Path $WorkRoot "7zr.exe"
$SevenZipExtraPath = Join-Path $WorkRoot "7z-extra.7z"
$SevenZipFullPath = Join-Path $WorkRoot "7z.exe"
$ExtractRoot = Join-Path $WorkRoot "extracted"

function Remove-TreeIfExists([string]$Path) {
    if (Test-Path $Path) {
        Remove-Item -LiteralPath $Path -Recurse -Force
    }
}

Get-Process -Name "gs10071w64" -ErrorAction SilentlyContinue | Stop-Process -Force

Remove-TreeIfExists $WorkRoot
Remove-TreeIfExists $BundleRoot
New-Item -ItemType Directory -Path $WorkRoot -Force | Out-Null

Write-Host "Downloading Ghostscript $GsVersion ..."
Invoke-WebRequest -Uri $GsUrl -OutFile $InstallerPath

Write-Host "Downloading 7-Zip tools ..."
Invoke-WebRequest -Uri $SevenZipUrl -OutFile $SevenZipPath
Invoke-WebRequest -Uri $SevenZipExtraUrl -OutFile $SevenZipExtraPath
& $SevenZipPath x $SevenZipExtraPath "-o$WorkRoot" -y | Out-Host
if (-not (Test-Path $SevenZipFullPath)) {
    throw "Failed to prepare 7z.exe extractor"
}

Write-Host "Extracting Ghostscript installer ..."
New-Item -ItemType Directory -Path $ExtractRoot -Force | Out-Null
& $SevenZipFullPath x $InstallerPath "-o$ExtractRoot" -y | Out-Host
if ($LASTEXITCODE -ne 0) {
    throw "Failed to extract Ghostscript installer"
}

$gsExe = Get-ChildItem -Path $ExtractRoot -Recurse -Filter "gswin64c.exe" |
    Select-Object -First 1
if (-not $gsExe) {
    throw "gswin64c.exe not found after extraction"
}

$GsRoot = $gsExe.Directory.Parent.FullName
$binDir = Join-Path $GsRoot "bin"
$resourceDir = Join-Path $GsRoot "Resource"

if (-not (Test-Path $resourceDir)) {
    $resourceDir = Get-ChildItem -Path $ExtractRoot -Recurse -Directory -Filter "Resource" |
        Select-Object -First 1 -ExpandProperty FullName
}

if (-not (Test-Path (Join-Path $binDir "gswin64c.exe"))) {
    $binDir = $gsExe.Directory.FullName
}

New-Item -ItemType Directory -Path (Join-Path $BundleRoot "bin") -Force | Out-Null
Copy-Item -Path (Join-Path $binDir "*") -Destination (Join-Path $BundleRoot "bin") -Recurse -Force
Copy-Item -Path $resourceDir -Destination (Join-Path $BundleRoot "Resource") -Recurse -Force

$licenseCandidates = @(
    (Join-Path $GsRoot "LICENSE"),
    (Join-Path $GsRoot "COPYING"),
    (Join-Path $ExtractRoot "LICENSE"),
    (Join-Path $ExtractRoot "COPYING")
)
foreach ($license in $licenseCandidates) {
    if (Test-Path $license) {
        Copy-Item -Path $license -Destination (Join-Path $BundleRoot "Ghostscript-LICENSE.txt") -Force
        break
    }
}

Remove-TreeIfExists $WorkRoot

Write-Host "Cleaning macOS metadata from Ghostscript bundle ..."
& (Join-Path $ScriptDir "clean-ghostscript-bundle.ps1") -BundleRoot $BundleRoot

Write-Host "Ghostscript bundle ready at: $BundleRoot"
& (Join-Path $BundleRoot "bin\gswin64c.exe") --version