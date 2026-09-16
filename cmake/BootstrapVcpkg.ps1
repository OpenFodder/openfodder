param(
    [Parameter(Mandatory=$true)][string]$Root
)
$ErrorActionPreference = 'Stop'
$projectRoot = Split-Path -Parent $PSScriptRoot
$revision = (Get-Content (Join-Path $projectRoot 'vcpkg-configuration.json') -Raw | ConvertFrom-Json).'default-registry'.baseline
if ($revision -notmatch '^[0-9a-f]{40}$') { throw 'Invalid vcpkg registry revision' }
$Root = [IO.Path]::GetFullPath($Root)
if ($env:VCPKG_DOWNLOADS) {
    New-Item -ItemType Directory -Path $env:VCPKG_DOWNLOADS -Force | Out-Null
}

if (-not (Test-Path -LiteralPath (Join-Path $Root '.git'))) {
    git clone --no-checkout --filter=blob:none https://github.com/microsoft/vcpkg.git $Root
    if ($LASTEXITCODE -ne 0) { throw 'Could not clone vcpkg' }
}
$current = git -C $Root rev-parse HEAD
if ($LASTEXITCODE -ne 0) { throw 'Could not read vcpkg revision' }
if ($current -ne $revision -or -not (Test-Path -LiteralPath (Join-Path $Root 'bootstrap-vcpkg.bat'))) {
    git -C $Root fetch --depth 1 origin $revision
    if ($LASTEXITCODE -ne 0) { throw 'Could not fetch pinned vcpkg revision' }
    git -C $Root checkout --detach $revision
    if ($LASTEXITCODE -ne 0) { throw 'Could not check out pinned vcpkg revision' }
}
$toolMetadata = ConvertFrom-StringData (Get-Content (Join-Path $Root 'scripts/vcpkg-tool-metadata.txt') -Raw)
$toolPath = Join-Path $Root 'vcpkg.exe'
if (Test-Path -LiteralPath $toolPath) {
    $toolVersion = & $toolPath version --disable-metrics
    if ($LASTEXITCODE -eq 0 -and ($toolVersion -join ' ').Contains("version $($toolMetadata.VCPKG_TOOL_RELEASE_TAG)-")) {
        Write-Output "Using pinned vcpkg $($toolMetadata.VCPKG_TOOL_RELEASE_TAG) at $Root"
        return
    }
}
& (Join-Path $Root 'bootstrap-vcpkg.bat') -disableMetrics
if ($LASTEXITCODE -ne 0) { throw 'Could not bootstrap vcpkg' }
