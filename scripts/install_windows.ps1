param(
    [string]$BinaryPath,
    [string]$InstallDir = "$env:LOCALAPPDATA\MeowLang\bin"
)

$ErrorActionPreference = "Stop"

$root = Resolve-Path "$PSScriptRoot\.."

if (-not $BinaryPath) {
    $candidates = @(
        (Join-Path $root "build\meow.exe"),
        (Join-Path $root "build\Release\meow.exe"),
        (Join-Path $root "build\Debug\meow.exe"),
        (Join-Path $root "meow.exe")
    )

    $BinaryPath = $candidates | Where-Object { Test-Path $_ } | Select-Object -First 1
}

if (-not $BinaryPath -or -not (Test-Path $BinaryPath)) {
    throw "Could not find meow.exe. Build first, or pass -BinaryPath <path-to-meow.exe>."
}

New-Item -ItemType Directory -Path $InstallDir -Force | Out-Null

$targetExe = Join-Path $InstallDir "meow.exe"
Copy-Item $BinaryPath $targetExe -Force

$currentUserPath = [Environment]::GetEnvironmentVariable("Path", "User")
$paths = @()
if ($currentUserPath) {
    $paths = $currentUserPath.Split(';') | Where-Object { $_ -ne "" }
}

if ($paths -notcontains $InstallDir) {
    $newPath = if ($currentUserPath) { "$currentUserPath;$InstallDir" } else { $InstallDir }
    [Environment]::SetEnvironmentVariable("Path", $newPath, "User")
    Write-Host "Added to User PATH: $InstallDir"
} else {
    Write-Host "Already in User PATH: $InstallDir"
}

Write-Host "Installed: $targetExe"
Write-Host "Open a NEW terminal, then run: meow --version"
