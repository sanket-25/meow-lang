param(
    [Parameter(Mandatory = $true)]
    [string]$Version,

    [string]$BuildDir = "build",
    [string]$OutDir = "dist"
)

$ErrorActionPreference = "Stop"

$root = Resolve-Path "$PSScriptRoot\.."
$buildPath = Join-Path $root $BuildDir
$outPath = Join-Path $root $OutDir

$exeCandidates = @(
    (Join-Path $buildPath "meow.exe"),
    (Join-Path $buildPath "Release\meow.exe"),
    (Join-Path $buildPath "Debug\meow.exe")
)

$exePath = $exeCandidates | Where-Object { Test-Path $_ } | Select-Object -First 1
if (-not $exePath) {
    throw "Could not find meow.exe in '$BuildDir'. Build first with CMake."
}

$packageName = "meowlang-$Version-windows-x64"
$packageDir = Join-Path $outPath $packageName
$zipPath = Join-Path $outPath "$packageName.zip"

if (Test-Path $packageDir) { Remove-Item $packageDir -Recurse -Force }
if (Test-Path $zipPath) { Remove-Item $zipPath -Force }

New-Item -ItemType Directory -Path $packageDir -Force | Out-Null

Copy-Item $exePath (Join-Path $packageDir "meow.exe")
if (Test-Path (Join-Path $root "meow.bat")) {
    Copy-Item (Join-Path $root "meow.bat") $packageDir
}
if (Test-Path (Join-Path $root "README.md")) {
    Copy-Item (Join-Path $root "README.md") $packageDir
}
if (Test-Path (Join-Path $root "examples")) {
    Copy-Item (Join-Path $root "examples") (Join-Path $packageDir "examples") -Recurse
}

Compress-Archive -Path (Join-Path $packageDir "*") -DestinationPath $zipPath
Write-Host "Created $zipPath"
