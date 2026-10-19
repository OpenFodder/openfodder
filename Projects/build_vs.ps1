param(
    [ValidateSet('x64', 'Win32')][string]$Platform = 'x64',
    [ValidateSet('Release', 'Debug')][string]$Configuration = 'Release',
    [ValidateSet('noffmpeg', 'ffmpeg')][string]$Video = 'ffmpeg',
    [string]$VcpkgRoot
)
$ErrorActionPreference = 'Stop'
$projectRoot = Split-Path -Parent $PSScriptRoot
if (-not $VcpkgRoot) { $VcpkgRoot = Join-Path $projectRoot 'build/vcpkg-tool' }
$VcpkgRoot = [IO.Path]::GetFullPath($VcpkgRoot)
& (Join-Path $projectRoot 'cmake/BootstrapVcpkg.ps1') -Root $VcpkgRoot

$vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio/Installer/vswhere.exe'
$instance = @(& $vswhere -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -format json | ConvertFrom-Json)[0]
if (-not $instance) { throw 'Visual Studio C++ build tools were not found.' }
$major = [int]$instance.installationVersion.Split('.')[0]
$generator = if ($major -ge 18) { 'Visual Studio 18 2026' } elseif ($major -eq 17) { 'Visual Studio 17 2022' } else { throw 'Visual Studio 2022 or newer is required.' }
$cmake = Get-Command cmake -ErrorAction SilentlyContinue
$cmakeExe = if ($cmake) { $cmake.Source } else {
    Join-Path $instance.installationPath 'Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe'
}
if (-not (Test-Path -LiteralPath $cmakeExe)) { throw 'CMake was not found.' }

$buildDir = Join-Path $PSScriptRoot "VS-vcpkg-$Platform-$Configuration-$Video"
$triplet = if ($Platform -eq 'Win32') { 'x86-windows' } else { 'x64-windows' }
if ($Configuration -eq 'Release') { $triplet += '-release' }
$ffmpeg = if ($Video -eq 'ffmpeg') { 'ON' } else { 'OFF' }
$configureArgs = @('-S', $projectRoot, '-B', $buildDir, '-G', $generator, '-A', $Platform,
    "-DCMAKE_TOOLCHAIN_FILE=$VcpkgRoot/scripts/buildsystems/vcpkg.cmake",
    "-DVCPKG_TARGET_TRIPLET=$triplet", "-DOPENFODDER_ENABLE_FFMPEG=$ffmpeg",
    "-DCMAKE_RUNTIME_OUTPUT_DIRECTORY=$buildDir/bin")
if ($Platform -eq 'Win32') { $configureArgs += '-DWITH_ASM=OFF' }
& $cmakeExe @configureArgs
if ($LASTEXITCODE -ne 0) { throw 'CMake configure failed.' }
& $cmakeExe --build $buildDir --config $Configuration -- /m
if ($LASTEXITCODE -ne 0) { throw 'Build failed.' }
Write-Output "Built $buildDir/bin/$Configuration/openfodder.exe. Deploy it with every DLL in that directory."
