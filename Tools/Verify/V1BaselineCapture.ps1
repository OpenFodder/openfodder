# Tools/Verify/V1BaselineCapture.ps1
# Phase 1 deliverable P1.5 - captures v1 generator baseline before the legacy strip.
# Spec: Documentation/Future/Phase0/D11_baseline_mechanics.md sections 3, 4, 4.1
# Wraps: RegenerateRandomMap.ps1 (campaign), inline mp pattern (multiplayer),
#        Tools/Verify/V1BaselineCapture.py (manifest/dedupe/verify aggregator).
# Determinism contract: per [[mapgen_determinism]], every (profile, seed) is
# generated TWICE and the two runs must hash-match. Mismatch aborts capture.
# Outputs: Tools/Verify/V1Baseline/seeds/<profile>/<seed>/record.json (per D11 sections 4 + 4.1)
#          Tools/Verify/V1Baseline/bytes/<sha256-prefix>/<full-sha256>.<ext>
#          Tools/Verify/V1Baseline/manifest.json

[CmdletBinding()]
param(
    [string[]]$Profiles = @('grammar_jungle', 'grammar_ice', 'grammar_beach', 'grammar_desert', 'grammar_moors', 'grammar_jungle_interior'),
    [string[]]$MpModes = @('deathmatch', 'rescue-prisoner'),
    [string[]]$MpTerrains = @('jungle', 'ice'),
    [uint32[]]$Seeds = @(),
    [uint32]$SeedStart = 101,
    [int]$SeedCount = 60,
    [string]$OutputDir = 'Tools/Verify/V1Baseline',
    [string]$ExePath = 'Run/openfodder.exe',
    [switch]$Rebaseline,
    [switch]$VerifyOnly,
    [switch]$SmokeOnly,
    [switch]$SkipMultiplayer,
    [switch]$SkipDeterminismCheck   # debug only
)

$ErrorActionPreference = 'Stop'
Set-StrictMode -Version Latest

# Resolve repo root
$RepoRoot = (Resolve-Path (Join-Path $PSScriptRoot '..\..')).Path
Set-Location $RepoRoot

# SmokeOnly override
if ($SmokeOnly) {
    $Profiles = @('grammar_jungle', 'grammar_ice')
    $Seeds = @()
    $SeedStart = 101
    $SeedCount = 5
    $SkipMultiplayer = $true
}

# Resolve OutputDir to absolute (under repo root)
if (-not [System.IO.Path]::IsPathRooted($OutputDir)) {
    $OutputDir = Join-Path $RepoRoot $OutputDir
}
New-Item -ItemType Directory -Force -Path $OutputDir | Out-Null
New-Item -ItemType Directory -Force -Path (Join-Path $OutputDir 'bytes') | Out-Null
New-Item -ItemType Directory -Force -Path (Join-Path $OutputDir 'seeds') | Out-Null

# Resolve ExePath to absolute
if (-not [System.IO.Path]::IsPathRooted($ExePath)) {
    $ExePath = Join-Path $RepoRoot $ExePath
}

# Helpers

function Get-Sha256Lower {
    param([string]$Path)
    return (Get-FileHash $Path -Algorithm SHA256).Hash.ToLower()
}

function Get-PropOrNull {
    param($Object, [string]$Name)
    if ($null -eq $Object) { return $null }
    if ($Object.PSObject.Properties[$Name]) {
        return $Object.$Name
    }
    return $null
}

function Invoke-PyDedupe {
    param(
        [string]$Hash,
        [string]$Source,
        [string]$Ext
    )
    $pyScript = Join-Path $RepoRoot 'Tools/Verify/V1BaselineCapture.py'
    $bytesDir = Join-Path $OutputDir 'bytes'
    & python $pyScript dedupe --bytes-dir $bytesDir --input $Hash --source $Source --ext $Ext
    if ($LASTEXITCODE -ne 0) {
        throw "dedupe failed for $Source -> $Hash.$Ext (exit $LASTEXITCODE)"
    }
}

function Get-EngineVersionFromMapgen {
    param($MapgenJson)
    $eng = Get-PropOrNull $MapgenJson 'engineVersion'
    if ($eng) { return $eng }
    $meta = Get-PropOrNull $MapgenJson 'meta'
    if ($meta) {
        $ev = Get-PropOrNull $meta 'engineVersion'
        if ($ev) { return $ev }
    }
    return $null
}

function Build-SeedList {
    # Always include the 5 RegressionGate seeds, plus the SeedStart..SeedCount range, plus explicit -Seeds
    $regressionSeeds = @([uint32]3156408432, [uint32]3156408433, [uint32]101, [uint32]202, [uint32]4242)
    $rangeSeeds = @()
    if ($SeedCount -gt 0) {
        $rangeSeeds = @($SeedStart..($SeedStart + $SeedCount - 1) | ForEach-Object { [uint32]$_ })
    }
    $merged = @()
    $merged += $regressionSeeds
    $merged += $rangeSeeds
    if ($Seeds -and $Seeds.Count -gt 0) {
        $merged += ($Seeds | ForEach-Object { [uint32]$_ })
    }
    # Dedup, preserve order
    $seen = @{}
    $out = @()
    foreach ($s in $merged) {
        if (-not $seen.ContainsKey($s)) {
            $seen[$s] = $true
            $out += $s
        }
    }
    return ,$out
}

function Invoke-RegenerateRandomMap {
    param(
        [string]$Profile,
        [uint32]$Seed,
        [string]$MapPath,
        [string]$MetadataPath
    )
    $script = Join-Path $RepoRoot 'Tools/Pipeline/RegenerateRandomMap.ps1'
    # -FastTileIteration is required: without it the engine hangs on the renderer pump even
    # under --headless, never writing the .map file. Pattern lifted from
    # Tools/Pipeline/RegressionGate.ps1:71 which also passes it for the same reason.
    & pwsh -NoProfile -File $script -Profile $Profile -Seed $Seed -MapPath $MapPath -MetadataPath $MetadataPath -FastTileIteration
    if ($LASTEXITCODE -ne 0) {
        throw "RegenerateRandomMap.ps1 failed for ${Profile}/${Seed} (exit $LASTEXITCODE)"
    }
}

function Invoke-MultiplayerGenerate {
    param(
        [string]$Mode,
        [string]$Terrain,
        [uint32]$Seed
    )
    & $ExePath --random --multiplayer --map-game-mode $Mode --multiplayer-terrain $Terrain --random-seed $Seed --map-generate-only --headless --nosound
    if ($LASTEXITCODE -ne 0) {
        throw "openfodder.exe multiplayer generate failed (mode=$Mode terrain=$Terrain seed=$Seed exit=$LASTEXITCODE)"
    }
}

function Invoke-RenderPng {
    param(
        [string]$MapPath,
        [string]$PngPath
    )
    & $ExePath --map-generate-png $MapPath --map-png-output $PngPath --map-png-scale 2 --amiga --engine cf1 --nosound --headless
    if ($LASTEXITCODE -ne 0) {
        throw "openfodder.exe --map-generate-png failed for $MapPath (exit $LASTEXITCODE)"
    }
}

function Build-Record {
    param(
        [string]$Profile,
        [uint32]$Seed,
        [string]$Kind,
        [string]$MpMode,
        [string]$MpTerrain,
        [string]$HashMap,
        [string]$HashSpt,
        [string]$HashPng,
        $HashIceCharmap,
        $ContextJson,
        $MapgenJson
    )

    $exeHash = Get-Sha256Lower $ExePath
    $engineVersion = Get-EngineVersionFromMapgen $MapgenJson

    # Pull terrain/terrainSub etc from context
    $terrain = $null
    $terrainSub = 0
    $width = $null
    $height = $null
    $selectedAttempt = 1
    $totalAttempts = 1
    $liveValidation = $null
    $routeSitePlan = $null
    $campaignFlowPlan = $null
    $spritePlan = $null
    $structurePlan = $null
    $metricsLandMasses = $null
    $metricsTreeFraction = $null
    $metricsWaterFraction = $null
    $metricsCoverFraction = $null
    $metricsCliffFraction = $null

    if ($ContextJson) {
        # context.json has top-level scalars `terrainType` ("ice"/"jungle"/...) and
        # `terrainTypeSub` (0/1). The `profile` field is the profile NAME string,
        # not a sub-object. Earlier code assumed `profile.terrain`/`profile.terrainSub`
        # which always returned null.
        $terrain = Get-PropOrNull $ContextJson 'terrainType'
        $tsub = Get-PropOrNull $ContextJson 'terrainTypeSub'
        if ($null -ne $tsub) { $terrainSub = $tsub }
        $width = Get-PropOrNull $ContextJson 'width'
        $height = Get-PropOrNull $ContextJson 'height'
        $sa = Get-PropOrNull $ContextJson 'selectedAttempt'
        if ($null -ne $sa) { $selectedAttempt = $sa }
        $attempts = Get-PropOrNull $ContextJson 'attempts'
        if ($attempts -and $attempts.Count) { $totalAttempts = $attempts.Count }
        $liveValidation = Get-PropOrNull $ContextJson 'validation'
        $routeSitePlan = Get-PropOrNull $ContextJson 'routeSitePlan'
        $campaignFlowPlan = Get-PropOrNull $ContextJson 'campaignFlowPlan'
        $spritePlan = Get-PropOrNull $ContextJson 'spritePlan'
        $structurePlan = Get-PropOrNull $ContextJson 'structurePlan'
        $metricsNode = Get-PropOrNull $ContextJson 'metrics'
        if ($metricsNode) {
            $metricsLandMasses = Get-PropOrNull $metricsNode 'landMasses'
            $metricsTreeFraction = Get-PropOrNull $metricsNode 'treeFraction'
            $metricsWaterFraction = Get-PropOrNull $metricsNode 'waterFraction'
            $metricsCoverFraction = Get-PropOrNull $metricsNode 'coverFraction'
            $metricsCliffFraction = Get-PropOrNull $metricsNode 'cliffFraction'
        }
    }

    # For MP captures, terrain comes from the MP terrain arg; mode is multiplayer
    $effectiveTerrain = $terrain
    if ($Kind -eq 'multiplayer') {
        $effectiveTerrain = $MpTerrain
    }

    $mapPrefix = $HashMap.Substring(0, 2)
    $sptPrefix = $HashSpt.Substring(0, 2)
    $pngPrefix = $HashPng.Substring(0, 2)
    $iceCharmapRef = $null
    if ($HashIceCharmap) {
        $icePrefix = $HashIceCharmap.Substring(0, 2)
        $iceCharmapRef = "../../../bytes/$icePrefix/$HashIceCharmap.txt"
    }

    $record = [ordered]@{
        schema            = 1
        tag               = 'v1-mapgen-baseline-pre-strip'
        capturedAtUtc     = (Get-Date -AsUTC -Format 'o')
        exe               = [ordered]@{
            path          = $ExePath
            sha256        = $exeHash
            engineVersion = $engineVersion
        }
        profile           = $Profile
        seed              = [uint32]$Seed
        kind              = $Kind
        multiplayerMode   = $MpMode
        terrain           = $effectiveTerrain
        terrainSub        = $terrainSub

        output            = [ordered]@{
            mapHash       = $HashMap
            sptHash       = $HashSpt
            renderHash    = $HashPng
            iceCharmapHash = $HashIceCharmap
            width         = $width
            height        = $height
            baseTileset   = $effectiveTerrain
            subTileset    = $effectiveTerrain
        }

        selectedAttempt   = $selectedAttempt
        selectedSeed      = [uint32]$Seed
        totalAttempts     = $totalAttempts
        liveValidation    = $liveValidation
        routeSitePlan     = $routeSitePlan
        campaignFlowPlan  = $campaignFlowPlan
        spritePlan        = $spritePlan
        structurePlan     = $structurePlan
        routeReachability = $null

        metrics           = [ordered]@{
            landMasses     = $metricsLandMasses
            treeFraction   = $metricsTreeFraction
            waterFraction  = $metricsWaterFraction
            coverFraction  = $metricsCoverFraction
            cliffFraction  = $metricsCliffFraction
        }

        files             = [ordered]@{
            map         = "../../../bytes/$mapPrefix/$HashMap.map"
            spt         = "../../../bytes/$sptPrefix/$HashSpt.spt"
            render      = "../../../bytes/$pngPrefix/$HashPng.png"
            iceCharmap  = $iceCharmapRef
            mapgenJson  = './mapgen.json'
            contextJson = './context.json'
        }

        # v3-only fields - emit nulls on v1 baseline records per D11 section 4.1
        conceptId          = $null
        conceptIntent      = $null
        conceptMatch       = $null
        bindingConstraints = @{}
    }

    return $record
}

function Save-Record {
    param(
        [hashtable]$State,
        [string]$Profile,
        [uint32]$Seed,
        $Record,
        [string]$ContextSrc,
        [string]$MapgenSrc
    )
    $seedDir = Join-Path $OutputDir "seeds/$Profile/$Seed"
    New-Item -ItemType Directory -Force -Path $seedDir | Out-Null
    $recordPath = Join-Path $seedDir 'record.json'
    ($Record | ConvertTo-Json -Depth 32) | Set-Content -Path $recordPath -Encoding utf8NoBOM
    if ($ContextSrc -and (Test-Path $ContextSrc)) {
        Copy-Item $ContextSrc (Join-Path $seedDir 'context.json') -Force
    }
    if ($MapgenSrc -and (Test-Path $MapgenSrc)) {
        Copy-Item $MapgenSrc (Join-Path $seedDir 'mapgen.json') -Force
    }
}

function Remove-Transient {
    param([string[]]$Paths)
    foreach ($p in $Paths) {
        if ($p -and (Test-Path $p)) {
            Remove-Item $p -Force -ErrorAction SilentlyContinue
        }
    }
}

function Capture-One {
    param(
        [hashtable]$State,
        [string]$Profile,
        [uint32]$Seed
    )

    $stem = "campaign_${Profile}_${Seed}"
    $tmpMap = Join-Path $RepoRoot "Run/${stem}.map"
    $tmpPng = Join-Path $RepoRoot "Run/${stem}.png"
    $mapgenSrc = Join-Path $RepoRoot "Run/${stem}.mapgen.json"
    # RegenerateRandomMap.ps1:274 copies the .spt next to the .map (ChangeExtension)
    # and Copy-IceCharmapForMap copies the icecharmap to <MapPath stem>.icecharmap.txt.
    # NOT Run/random.spt and NOT Run/icecharmap_<seed>.txt (those are the engine's
    # raw output paths, but RegenerateRandomMap renames them to match $MapPath stem).
    $tmpSpt = Join-Path $RepoRoot "Run/${stem}.spt"
    $tmpIceCharmap = Join-Path $RepoRoot "Run/${stem}.icecharmap.txt"
    $contextSrc = Join-Path $RepoRoot "Run/mapgen_context_${Seed}.json"

    Write-Host "[capture] $stem" -ForegroundColor Cyan

    try {
        # Run #1
        Invoke-RegenerateRandomMap -Profile $Profile -Seed $Seed -MapPath $tmpMap -MetadataPath $mapgenSrc
        if (-not (Test-Path $tmpMap)) { throw "Generation produced no map: $tmpMap" }
        $hash1Map = Get-Sha256Lower $tmpMap
        $hash1Spt = if (Test-Path $tmpSpt) { Get-Sha256Lower $tmpSpt } else { throw "Missing $tmpSpt after generation" }

        # Run #2 (determinism check) unless suppressed
        if (-not $SkipDeterminismCheck) {
            Invoke-RegenerateRandomMap -Profile $Profile -Seed $Seed -MapPath $tmpMap -MetadataPath $mapgenSrc
            $hash2Map = Get-Sha256Lower $tmpMap
            $hash2Spt = if (Test-Path $tmpSpt) { Get-Sha256Lower $tmpSpt } else { throw "Missing $tmpSpt after rerun" }
            if ($hash1Map -ne $hash2Map) {
                throw "DETERMINISM FAILURE: $stem map hash diverges between runs ($hash1Map vs $hash2Map)"
            }
            if ($hash1Spt -ne $hash2Spt) {
                throw "DETERMINISM FAILURE: $stem spt hash diverges between runs ($hash1Spt vs $hash2Spt)"
            }
        }

        # Render PNG
        Invoke-RenderPng -MapPath $tmpMap -PngPath $tmpPng
        $hashPng = Get-Sha256Lower $tmpPng

        # IceCharmap sidecar (ice profiles only — RegenerateRandomMap copies it next to MapPath)
        $hashIceCharmap = $null
        if (Test-Path $tmpIceCharmap) {
            $hashIceCharmap = Get-Sha256Lower $tmpIceCharmap
        }

        # Dedupe each artefact via Python
        Invoke-PyDedupe -Hash $hash1Map -Source $tmpMap -Ext 'map'
        Invoke-PyDedupe -Hash $hash1Spt -Source $tmpSpt -Ext 'spt'
        Invoke-PyDedupe -Hash $hashPng -Source $tmpPng -Ext 'png'
        if ($hashIceCharmap) {
            Invoke-PyDedupe -Hash $hashIceCharmap -Source $tmpIceCharmap -Ext 'txt'
        }

        # Read sidecars
        $contextJson = $null
        if (Test-Path $contextSrc) {
            $contextJson = Get-Content $contextSrc -Raw | ConvertFrom-Json -Depth 32
        }
        $mapgenJson = $null
        if (Test-Path $mapgenSrc) {
            $mapgenJson = Get-Content $mapgenSrc -Raw | ConvertFrom-Json -Depth 32
        }

        $record = Build-Record -Profile $Profile -Seed $Seed -Kind 'campaign' -MpMode $null -MpTerrain $null `
            -HashMap $hash1Map -HashSpt $hash1Spt -HashPng $hashPng -HashIceCharmap $hashIceCharmap `
            -ContextJson $contextJson -MapgenJson $mapgenJson

        Save-Record -State $State -Profile $Profile -Seed $Seed -Record $record `
            -ContextSrc $contextSrc -MapgenSrc $mapgenSrc

        $State.Passed++
        Write-Host "  [ok] $stem map=$($hash1Map.Substring(0,12)) png=$($hashPng.Substring(0,12))" -ForegroundColor Green
    } catch {
        $State.Failed++
        $State.Failures += "${stem}: $($_.Exception.Message)"
        Write-Host "  [FAIL] $stem - $($_.Exception.Message)" -ForegroundColor Red
    } finally {
        Remove-Transient -Paths @($tmpMap, $tmpPng, $tmpSpt, $tmpIceCharmap, $mapgenSrc)
    }
}

function Capture-OneMp {
    param(
        [hashtable]$State,
        [string]$MpMode,
        [string]$MpTerrain,
        [uint32]$Seed
    )

    $stem = "multiplayer_${MpMode}_${MpTerrain}_${Seed}"
    $randomMap = Join-Path $RepoRoot 'Run/random.map'
    $randomSpt = Join-Path $RepoRoot 'Run/random.spt'
    $tmpMap = Join-Path $RepoRoot "Run/${stem}.map"
    $tmpPng = Join-Path $RepoRoot "Run/${stem}.png"
    $contextSrc = Join-Path $RepoRoot "Run/mapgen_context_${Seed}.json"
    $iceCharmapSrc = Join-Path $RepoRoot "Run/icecharmap_${Seed}.txt"

    # Use the synthetic profile id "mp_<mode>_<terrain>" so that record dirs don't
    # collide with campaign captures; D11 records still carry kind=multiplayer + multiplayerMode.
    $profileId = "mp_${MpMode}_${MpTerrain}"

    Write-Host "[capture-mp] $stem" -ForegroundColor Cyan

    try {
        # Run #1
        Invoke-MultiplayerGenerate -Mode $MpMode -Terrain $MpTerrain -Seed $Seed
        if (-not (Test-Path $randomMap)) { throw "MP generation produced no map: $randomMap" }
        Copy-Item $randomMap $tmpMap -Force
        $hash1Map = Get-Sha256Lower $tmpMap
        $hash1Spt = if (Test-Path $randomSpt) { Get-Sha256Lower $randomSpt } else { throw "Missing $randomSpt after MP generation" }

        # Run #2 (determinism check) unless suppressed
        if (-not $SkipDeterminismCheck) {
            Invoke-MultiplayerGenerate -Mode $MpMode -Terrain $MpTerrain -Seed $Seed
            $hash2Map = if (Test-Path $randomMap) { Get-Sha256Lower $randomMap } else { throw "Missing $randomMap after MP rerun" }
            $hash2Spt = if (Test-Path $randomSpt) { Get-Sha256Lower $randomSpt } else { throw "Missing $randomSpt after MP rerun" }
            if ($hash1Map -ne $hash2Map) {
                throw "DETERMINISM FAILURE: $stem map hash diverges between runs ($hash1Map vs $hash2Map)"
            }
            if ($hash1Spt -ne $hash2Spt) {
                throw "DETERMINISM FAILURE: $stem spt hash diverges between runs ($hash1Spt vs $hash2Spt)"
            }
        }

        # Render PNG
        Invoke-RenderPng -MapPath $tmpMap -PngPath $tmpPng
        $hashPng = Get-Sha256Lower $tmpPng

        # IceCharmap sidecar
        $hashIceCharmap = $null
        if (Test-Path $iceCharmapSrc) {
            $hashIceCharmap = Get-Sha256Lower $iceCharmapSrc
        }

        # Dedupe each artefact via Python
        Invoke-PyDedupe -Hash $hash1Map -Source $tmpMap -Ext 'map'
        Invoke-PyDedupe -Hash $hash1Spt -Source $randomSpt -Ext 'spt'
        Invoke-PyDedupe -Hash $hashPng -Source $tmpPng -Ext 'png'
        if ($hashIceCharmap) {
            Invoke-PyDedupe -Hash $hashIceCharmap -Source $iceCharmapSrc -Ext 'txt'
        }

        # Read sidecars
        $contextJson = $null
        if (Test-Path $contextSrc) {
            $contextJson = Get-Content $contextSrc -Raw | ConvertFrom-Json -Depth 32
        }

        $record = Build-Record -Profile $profileId -Seed $Seed -Kind 'multiplayer' -MpMode $MpMode -MpTerrain $MpTerrain `
            -HashMap $hash1Map -HashSpt $hash1Spt -HashPng $hashPng -HashIceCharmap $hashIceCharmap `
            -ContextJson $contextJson -MapgenJson $null

        Save-Record -State $State -Profile $profileId -Seed $Seed -Record $record `
            -ContextSrc $contextSrc -MapgenSrc $null

        $State.Passed++
        Write-Host "  [ok] $stem map=$($hash1Map.Substring(0,12)) png=$($hashPng.Substring(0,12))" -ForegroundColor Green
    } catch {
        $State.Failed++
        $State.Failures += "${stem}: $($_.Exception.Message)"
        Write-Host "  [FAIL] $stem - $($_.Exception.Message)" -ForegroundColor Red
    } finally {
        Remove-Transient -Paths @($tmpMap, $tmpPng, $randomMap, $randomSpt)
    }
}

function Verify-One {
    param(
        [hashtable]$State,
        [string]$Profile,
        [uint32]$Seed
    )
    $stem = "campaign_${Profile}_${Seed}"
    $recordPath = Join-Path $OutputDir "seeds/$Profile/$Seed/record.json"
    $tmpMap = Join-Path $RepoRoot "Run/verify_${stem}.map"
    $tmpPng = Join-Path $RepoRoot "Run/verify_${stem}.png"
    $mapgenTmp = Join-Path $RepoRoot "Run/verify_${stem}.mapgen.json"
    $sptRandom = Join-Path $RepoRoot 'Run/random.spt'

    Write-Host "[verify] $stem" -ForegroundColor Cyan

    try {
        if (-not (Test-Path $recordPath)) { throw "Missing record: $recordPath" }
        $record = Get-Content $recordPath -Raw | ConvertFrom-Json -Depth 32

        Invoke-RegenerateRandomMap -Profile $Profile -Seed $Seed -MapPath $tmpMap -MetadataPath $mapgenTmp
        $hashMap = Get-Sha256Lower $tmpMap
        if ($hashMap -ne $record.output.mapHash) {
            throw "VERIFY FAILURE: $stem map hash $hashMap != recorded $($record.output.mapHash)"
        }
        $hashSpt = Get-Sha256Lower $sptRandom
        if ($hashSpt -ne $record.output.sptHash) {
            throw "VERIFY FAILURE: $stem spt hash $hashSpt != recorded $($record.output.sptHash)"
        }
        Invoke-RenderPng -MapPath $tmpMap -PngPath $tmpPng
        $hashPng = Get-Sha256Lower $tmpPng
        if ($hashPng -ne $record.output.renderHash) {
            throw "VERIFY FAILURE: $stem render hash $hashPng != recorded $($record.output.renderHash)"
        }

        # Defer to Python aggregator for filesystem-level checks
        $pyScript = Join-Path $RepoRoot 'Tools/Verify/V1BaselineCapture.py'
        & python $pyScript verify --baseline-dir $OutputDir --record-path "seeds/$Profile/$Seed/record.json"
        if ($LASTEXITCODE -ne 0) {
            throw "Python verify failed for $stem (exit $LASTEXITCODE)"
        }

        $State.Passed++
        Write-Host "  [ok] verified $stem" -ForegroundColor Green
    } catch {
        $State.Failed++
        $State.Failures += "${stem}: $($_.Exception.Message)"
        Write-Host "  [FAIL] $stem - $($_.Exception.Message)" -ForegroundColor Red
    } finally {
        Remove-Transient -Paths @($tmpMap, $tmpPng, $mapgenTmp, $sptRandom)
    }
}

function Verify-OneMp {
    param(
        [hashtable]$State,
        [string]$MpMode,
        [string]$MpTerrain,
        [uint32]$Seed
    )
    $profileId = "mp_${MpMode}_${MpTerrain}"
    $stem = "multiplayer_${MpMode}_${MpTerrain}_${Seed}"
    $recordPath = Join-Path $OutputDir "seeds/$profileId/$Seed/record.json"
    $randomMap = Join-Path $RepoRoot 'Run/random.map'
    $randomSpt = Join-Path $RepoRoot 'Run/random.spt'
    $tmpPng = Join-Path $RepoRoot "Run/verify_${stem}.png"

    Write-Host "[verify-mp] $stem" -ForegroundColor Cyan

    try {
        if (-not (Test-Path $recordPath)) { throw "Missing record: $recordPath" }
        $record = Get-Content $recordPath -Raw | ConvertFrom-Json -Depth 32

        Invoke-MultiplayerGenerate -Mode $MpMode -Terrain $MpTerrain -Seed $Seed
        $hashMap = Get-Sha256Lower $randomMap
        if ($hashMap -ne $record.output.mapHash) {
            throw "VERIFY FAILURE: $stem map hash $hashMap != recorded $($record.output.mapHash)"
        }
        $hashSpt = Get-Sha256Lower $randomSpt
        if ($hashSpt -ne $record.output.sptHash) {
            throw "VERIFY FAILURE: $stem spt hash $hashSpt != recorded $($record.output.sptHash)"
        }
        Invoke-RenderPng -MapPath $randomMap -PngPath $tmpPng
        $hashPng = Get-Sha256Lower $tmpPng
        if ($hashPng -ne $record.output.renderHash) {
            throw "VERIFY FAILURE: $stem render hash $hashPng != recorded $($record.output.renderHash)"
        }

        $pyScript = Join-Path $RepoRoot 'Tools/Verify/V1BaselineCapture.py'
        & python $pyScript verify --baseline-dir $OutputDir --record-path "seeds/$profileId/$Seed/record.json"
        if ($LASTEXITCODE -ne 0) {
            throw "Python verify failed for $stem (exit $LASTEXITCODE)"
        }

        $State.Passed++
        Write-Host "  [ok] verified $stem" -ForegroundColor Green
    } catch {
        $State.Failed++
        $State.Failures += "${stem}: $($_.Exception.Message)"
        Write-Host "  [FAIL] $stem - $($_.Exception.Message)" -ForegroundColor Red
    } finally {
        Remove-Transient -Paths @($tmpPng, $randomMap, $randomSpt)
    }
}

# Main

$state = @{
    Passed = 0
    Failed = 0
    Failures = @()
}

$seedList = Build-SeedList
Write-Host "Seed list ($(($seedList | Measure-Object).Count) total): $($seedList -join ', ')" -ForegroundColor DarkGray
Write-Host "Profiles: $($Profiles -join ', ')" -ForegroundColor DarkGray
if (-not $SkipMultiplayer) {
    Write-Host "MP modes: $($MpModes -join ', '); MP terrains: $($MpTerrains -join ', ')" -ForegroundColor DarkGray
}

# Verify pre-reqs
if (-not (Test-Path $ExePath)) {
    Write-Error "openfodder.exe not found at $ExePath"
    exit 2
}

# Campaign captures
foreach ($profile in $Profiles) {
    foreach ($seed in $seedList) {
        if ($VerifyOnly) {
            Verify-One -State $state -Profile $profile -Seed $seed
        } else {
            $seedDir = Join-Path $OutputDir "seeds/$profile/$seed"
            $existing = Join-Path $seedDir 'record.json'
            if ((Test-Path $existing) -and -not $Rebaseline) {
                Write-Host "[skip] campaign_${profile}_${seed} (record.json exists; use -Rebaseline to overwrite)" -ForegroundColor DarkYellow
                continue
            }
            Capture-One -State $state -Profile $profile -Seed $seed
        }
    }
}

# Multiplayer captures
if (-not $SkipMultiplayer) {
    foreach ($mpMode in $MpModes) {
        foreach ($mpTerrain in $MpTerrains) {
            foreach ($seed in $seedList) {
                if ($VerifyOnly) {
                    Verify-OneMp -State $state -MpMode $mpMode -MpTerrain $mpTerrain -Seed $seed
                } else {
                    $profileId = "mp_${mpMode}_${mpTerrain}"
                    $seedDir = Join-Path $OutputDir "seeds/$profileId/$seed"
                    $existing = Join-Path $seedDir 'record.json'
                    if ((Test-Path $existing) -and -not $Rebaseline) {
                        Write-Host "[skip] multiplayer_${mpMode}_${mpTerrain}_${seed} (record.json exists; use -Rebaseline to overwrite)" -ForegroundColor DarkYellow
                        continue
                    }
                    Capture-OneMp -State $state -MpMode $mpMode -MpTerrain $mpTerrain -Seed $seed
                }
            }
        }
    }
}

# Manifest + probe aggregation (skip if VerifyOnly)
if (-not $VerifyOnly) {
    Write-Host "[manifest] writing manifest.json" -ForegroundColor Cyan
    $tagSha = ''
    try {
        $tagSha = (& git rev-list -n 1 v1-mapgen-baseline-pre-strip 2>$null | Out-String).Trim()
    } catch {
        $tagSha = ''
    }
    if ([string]::IsNullOrWhiteSpace($tagSha)) {
        $tagSha = '<tag-not-yet-created>'
    }
    $exeSha = Get-Sha256Lower $ExePath
    $capturedAt = (Get-Date -AsUTC -Format 'o')
    $pyScript = Join-Path $RepoRoot 'Tools/Verify/V1BaselineCapture.py'
    $manifestOut = Join-Path $OutputDir 'manifest.json'

    & python $pyScript manifest `
        --baseline-dir $OutputDir `
        --tag-sha $tagSha `
        --exe-sha $exeSha `
        --captured-at-utc $capturedAt `
        --output $manifestOut
    if ($LASTEXITCODE -ne 0) {
        Write-Host "[manifest] FAILED (python exit $LASTEXITCODE)" -ForegroundColor Red
        $state.Failed++
        $state.Failures += "manifest: python exit $LASTEXITCODE"
    } else {
        Write-Host "[manifest] -> $manifestOut" -ForegroundColor Green
    }

    # Aggregate probes if any audit/probes/*.json exist under repo
    $probesGlob = Join-Path $RepoRoot 'audit/probes/*.json'
    $probeFiles = @(Get-ChildItem -Path $probesGlob -ErrorAction SilentlyContinue)
    if ($probeFiles.Count -gt 0) {
        Write-Host "[probes] aggregating $($probeFiles.Count) probe file(s)" -ForegroundColor Cyan
        & python $pyScript aggregate-probes --baseline-dir $OutputDir --probes-dir (Join-Path $RepoRoot 'audit/probes')
        if ($LASTEXITCODE -ne 0) {
            Write-Host "[probes] aggregate-probes failed (exit $LASTEXITCODE)" -ForegroundColor Red
            $state.Failed++
            $state.Failures += "aggregate-probes: python exit $LASTEXITCODE"
        }
    }
}

# Summary
$total = $state.Passed + $state.Failed
Write-Host ''
Write-Host '=== V1 Baseline Capture Summary ===' -ForegroundColor White
Write-Host "  total:  $total"
Write-Host "  passed: $($state.Passed)" -ForegroundColor Green
Write-Host "  failed: $($state.Failed)" -ForegroundColor ($(if ($state.Failed -gt 0) { 'Red' } else { 'DarkGray' }))
if ($state.Failed -gt 0) {
    Write-Host '  failures:' -ForegroundColor Red
    foreach ($f in $state.Failures) {
        Write-Host "    - $f" -ForegroundColor Red
    }
}

# Dedup ratio (#bytes files vs #captures*4 expected artefacts approx)
$bytesDir = Join-Path $OutputDir 'bytes'
if (Test-Path $bytesDir) {
    $bytesCount = @(Get-ChildItem -Path $bytesDir -Recurse -File -ErrorAction SilentlyContinue).Count
    if ($total -gt 0) {
        $expected = $total * 3   # map + spt + png
        $ratio = if ($expected -gt 0) { [math]::Round($bytesCount / [double]$expected, 3) } else { 0 }
        Write-Host "  dedup:  $bytesCount unique byte files / ~$expected raw artefacts (ratio $ratio)"
    } else {
        Write-Host "  dedup:  $bytesCount unique byte files"
    }
}

if ($state.Failed -gt 0) {
    exit 1
}
exit 0
