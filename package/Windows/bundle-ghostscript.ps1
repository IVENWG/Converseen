# Downloads and stages Ghostscript for the LessMB Windows portable bundle.
# Run from repo root or this script's directory before `cmake --install`.
#
# Output layout (next to lessmb.exe after install):
#   ghostscript/bin/gswin64c.exe
#   ghostscript/Resource/...

$ErrorActionPreference = "Stop"

$GsVersion = "10.07.1"
$GsTag = "gs10071"
$GsInstaller = "gs10071w64.exe"
$GsUrl = "https://github.com/ArtifexSoftware/ghostpdl-downloads/releases/download/$GsTag/$GsInstaller"

$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$BundleRoot = Join-Path $ScriptDir "ghostscript"
$StageRoot = Join-Path $ScriptDir "_ghostscript_stage"
$InstallerPath = Join-Path $env:TEMP $GsInstaller

function Remove-TreeIfExists([string]$Path) {
    if (Test-Path $Path) {
        Remove-Item -LiteralPath $Path -Recurse -Force
    }
}

Write-Host "Downloading Ghostscript $GsVersion ..."
Invoke-WebRequest -Uri $GsUrl -OutFile $InstallerPath

Remove-TreeIfExists $StageRoot
New-Item -ItemType Directory -Path $StageRoot | Out-Null

Write-Host "Installing Ghostscript to staging directory ..."
$installArgs = @(
    "/S",
    "/D=$StageRoot"
)
$process = Start-Process -FilePath $InstallerPath -ArgumentList $installArgs -Wait -PassThru
if ($process.ExitCode -ne 0) {
    throw "Ghostscript installer failed with exit code $($process.ExitCode)"
}

$GsRoot = $StageRoot
$versionDir = Get-ChildItem -Path $StageRoot -Directory |
    Where-Object { $_.Name -match '^gs\d' } |
    Select-Object -First 1

if ($versionDir) {
    $GsRoot = $versionDir.FullName
}

$binDir = Join-Path $GsRoot "bin"
$resourceDir = Join-Path $GsRoot "Resource"
$gsExe = Join-Path $binDir "gswin64c.exe"

if (-not (Test-Path $gsExe)) {
    throw "Expected Ghostscript binary not found: $gsExe"
}

Remove-TreeIfExists $BundleRoot
New-Item -ItemType Directory -Path (Join-Path $BundleRoot "bin") | Out-Null
Copy-Item -Path (Join-Path $binDir "*") -Destination (Join-Path $BundleRoot "bin") -Recurse -Force
Copy-Item -Path $resourceDir -Destination (Join-Path $BundleRoot "Resource") -Recurse -Force

$licenseCandidates = @(
    (Join-Path $GsRoot "LICENSE"),
    (Join-Path $GsRoot "COPYING"),
    (Join-Path $GsRoot "doc\COPYING")
)
foreach ($license in $licenseCandidates) {
    if (Test-Path $license) {
        Copy-Item -Path $license -Destination (Join-Path $BundleRoot "Ghostscript-LICENSE.txt") -Force
        break
    }
}

Remove-TreeIfExists $StageRoot
Remove-Item -LiteralPath $InstallerPath -Force -ErrorAction SilentlyContinue

Write-Host "Ghostscript bundle ready at: $BundleRoot"
& $gsExe --version