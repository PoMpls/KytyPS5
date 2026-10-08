param(
    [string]$Game = (Join-Path ([Environment]::GetFolderPath('Desktop')) 'ps5/ASTRO-BOT.zar'),
    [string]$UserName = $env:USERNAME
)
$ErrorActionPreference = 'Stop'
$runtime = $PSScriptRoot
$emulator = Join-Path $runtime 'kyty_emulator.exe'
if (-not (Test-Path -LiteralPath $emulator)) { throw 'Place this script next to kyty_emulator.exe.' }
if (-not (Test-Path -LiteralPath $Game)) { throw "Game not found: $Game" }
$Game = (Resolve-Path -LiteralPath $Game).Path
if ($Game.Contains('"') -or $UserName.Contains('"')) { throw 'Unexpected quote in a launch argument.' }
Get-ChildItem Env: | Where-Object { $_.Name -match '^(KYTY_|TRACY_)' } | ForEach-Object {
    [Environment]::SetEnvironmentVariable($_.Name, $null, 'Process')
}
$preset = Get-Content -LiteralPath (Join-Path $runtime 'u59-preset.json') -Raw | ConvertFrom-Json
foreach ($property in $preset.PSObject.Properties) {
    if ($property.Name -notmatch '^(KYTY_|TRACY_)' -or $property.Value -isnot [string]) {
        throw 'Invalid runtime preset.'
    }
    [Environment]::SetEnvironmentVariable($property.Name, $property.Value, 'Process')
}
$logs = Join-Path $runtime ('test-' + (Get-Date -Format 'yyyyMMdd-HHmmss'))
New-Item -ItemType Directory -Path $logs | Out-Null
$arguments = @('--game', ('"{0}"' -f $Game), '--console-language', '3',
    '--user-name', ('"{0}"' -f $UserName), '--redzone', '--present-mode', 'Immediate',
    '--vblank-frequency', '60', '--screen-width', '1920', '--screen-height', '1080',
    '--shader-optimization-type', 'Performance', '--shader-validation', 'true',
    '--printf-direction', 'Silent', '--shader-log-direction', 'Silent')
# Use the original lighting shaders without --game-patch.
$arguments | ConvertTo-Json | Set-Content -LiteralPath (Join-Path $logs 'arguments.json')
Get-FileHash -LiteralPath $emulator | ConvertTo-Json | Set-Content -LiteralPath (Join-Path $logs 'executable.json')
Get-Content -LiteralPath (Join-Path $runtime 'u59-preset.json') | Set-Content -LiteralPath (Join-Path $logs 'preset.json')
$gameProcess = Start-Process -FilePath $emulator -ArgumentList $arguments -WorkingDirectory $runtime -WindowStyle Hidden -RedirectStandardOutput (Join-Path $logs 'stdout.log') -RedirectStandardError (Join-Path $logs 'stderr.log') -PassThru
$gameProcess.Id | Set-Content -LiteralPath (Join-Path $logs 'process-id.txt')
Write-Output "Astro Bot started: PID $($gameProcess.Id); logs: $logs"
$gameProcess.WaitForExit()
$gameProcess.ExitCode | Set-Content -LiteralPath (Join-Path $logs 'exit-code.txt')
Write-Output "Emulator exit code: $($gameProcess.ExitCode)"
exit $gameProcess.ExitCode
