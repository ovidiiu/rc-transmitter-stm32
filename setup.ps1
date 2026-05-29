# Run from project root: C:\Projects\rc-transmitter-stm32\

# Create folder structure
$folders = @(
    "src\crsf",
    "src\input",
    "src\mixer",
    "src\ui",
    "src\audio",
    ".vscode"
)

foreach ($f in $folders) {
    New-Item -ItemType Directory -Force -Path $f
}

Write-Host "✅ Folder structure created!" -ForegroundColor Green
Write-Host ""
Write-Host "Now create these files with the source code:"
Write-Host "  platformio.ini"
Write-Host "  src\config.h"
Write-Host "  src\main.cpp"
Write-Host "  src\crsf\crsf.h"
Write-Host "  src\crsf\crsf.cpp"
Write-Host "  src\input\adc.h"
Write-Host "  src\input\adc.cpp"
Write-Host "  src\input\switches.h"
Write-Host "  src\input\switches.cpp"
Write-Host "  src\mixer\mixer.h"
Write-Host "  src\mixer\mixer.cpp"
Write-Host "  src\ui\display.h"
Write-Host "  src\ui\display.cpp"
Write-Host "  src\audio\buzzer.h"
Write-Host "  src\audio\buzzer.cpp"
