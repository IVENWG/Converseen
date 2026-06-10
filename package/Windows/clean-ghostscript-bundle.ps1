# Removes macOS metadata files accidentally bundled with Ghostscript.

param(
    [Parameter(Mandatory = $true)]
    [string]$BundleRoot
)

$ErrorActionPreference = "Stop"

if (-not (Test-Path -LiteralPath $BundleRoot)) {
    throw "Ghostscript bundle not found: $BundleRoot"
}

$removed = 0
Get-ChildItem -LiteralPath $BundleRoot -Recurse -Force |
    Where-Object { $_.Name -like '._*' -or $_.Name -eq '.DS_Store' } |
    ForEach-Object {
        Remove-Item -LiteralPath $_.FullName -Force
        $removed++
    }

Write-Host "Removed $removed metadata file(s) from $BundleRoot"