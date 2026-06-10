# Verifies ImageMagick can read a PDF page using the bundled Ghostscript.

param(
    [Parameter(Mandatory = $true)]
    [string]$ReleaseDir
)

$ErrorActionPreference = "Stop"

$gsBin = Join-Path $ReleaseDir "ghostscript\bin"
$gsInit = Join-Path $ReleaseDir "ghostscript\Resource\Init"
$pdfPath = Join-Path $ReleaseDir "_pdf_smoke_test.pdf"
$pdfUrl = "https://www.w3.org/WAI/ER/tests/xhtml/testfiles/resources/pdf/dummy.pdf"

if (-not (Test-Path (Join-Path $gsBin "gswin64c.exe"))) {
    throw "Bundled gswin64c.exe not found"
}

$magickCandidates = @(
    "C:\ImageMagick-64\magick.exe",
    "magick.exe"
)

$magick = $magickCandidates | Where-Object { Test-Path $_ } | Select-Object -First 1
if (-not $magick) {
    $magick = (Get-Command magick.exe -ErrorAction SilentlyContinue).Source
}
if (-not $magick) {
    throw "ImageMagick magick.exe not found for PDF smoke test"
}

Invoke-WebRequest -Uri $pdfUrl -OutFile $pdfPath

$env:PATH = "$gsBin;$env:PATH"
$env:MAGICK_CONFIGURE_PATH = $ReleaseDir
$env:MAGICK_CODER_MODULE_PATH = Join-Path $ReleaseDir "modules\coders"
$env:GS_LIB = $gsInit

$output = & $magick identify $pdfPath 2>&1
if ($LASTEXITCODE -ne 0) {
    Write-Host $output
    throw "PDF smoke test failed"
}

Write-Host "PDF_TEST_OK"
Remove-Item -LiteralPath $pdfPath -Force -ErrorAction SilentlyContinue