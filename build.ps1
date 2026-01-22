# SmartPark CLI Build Script
# This script compiles the modular C++ parking system

Write-Host "=============================================" -ForegroundColor Cyan
Write-Host "  SmartPark CLI Build Script" -ForegroundColor Cyan
Write-Host "=============================================" -ForegroundColor Cyan
Write-Host ""

# Set build directory
$buildDir = "build/cli"
$srcDir = "src"

# Create build directory if it doesn't exist
Write-Host "Creating build directory..." -ForegroundColor Yellow
if (!(Test-Path -Path $buildDir)) {
    New-Item -ItemType Directory -Path $buildDir -Force | Out-Null
    Write-Host "Build directory created: $buildDir" -ForegroundColor Green
} else {
    Write-Host "Build directory already exists: $buildDir" -ForegroundColor Green
}
Write-Host ""

# Compiler settings
$compiler = "g++"
$cppStandard = "-std=c++11"
$outputExe = "$buildDir/smartpark.exe"

# Source files
$libFiles = @(
    "$srcDir/lib/RequestState.cpp",
    "$srcDir/lib/ParkingSlot.cpp",
    "$srcDir/lib/ParkingArea.cpp",
    "$srcDir/lib/Zone.cpp",
    "$srcDir/lib/Vehicle.cpp",
    "$srcDir/lib/ParkingRequest.cpp",
    "$srcDir/lib/Operation.cpp",
    "$srcDir/lib/RollbackManager.cpp",
    "$srcDir/lib/AllocationEngine.cpp",
    "$srcDir/lib/AnalyticsEngine.cpp",
    "$srcDir/lib/ParkingSystem.cpp"
)

$mainFile = "$srcDir/main.cpp"

# Check if compiler exists
Write-Host "Checking for g++ compiler..." -ForegroundColor Yellow
try {
    $compilerVersion = & $compiler --version 2>&1 | Select-Object -First 1
    Write-Host "Compiler found: $compilerVersion" -ForegroundColor Green
} catch {
    Write-Host "Error: g++ compiler not found!" -ForegroundColor Red
    Write-Host "Please install MinGW or a C++ compiler and add it to your PATH." -ForegroundColor Red
    exit 1
}
Write-Host ""

# Check if all source files exist
Write-Host "Checking source files..." -ForegroundColor Yellow
$allFilesExist = $true
foreach ($file in $libFiles) {
    if (!(Test-Path -Path $file)) {
        Write-Host "  [ERROR] Missing: $file" -ForegroundColor Red
        $allFilesExist = $false
    } else {
        Write-Host "  [OK] $file" -ForegroundColor Green
    }
}

if (!(Test-Path -Path $mainFile)) {
    Write-Host "  [ERROR] Missing: $mainFile" -ForegroundColor Red
    $allFilesExist = $false
} else {
    Write-Host "  [OK] $mainFile" -ForegroundColor Green
}

if (!$allFilesExist) {
    Write-Host ""
    Write-Host "Build failed: Missing source files!" -ForegroundColor Red
    exit 1
}
Write-Host ""

# Compile the project
Write-Host "Compiling SmartPark..." -ForegroundColor Yellow
Write-Host "Command: $compiler $cppStandard $mainFile $($libFiles -join ' ') -o $outputExe" -ForegroundColor Gray
Write-Host ""

try {
    $allFiles = @($mainFile) + $libFiles
    & $compiler $cppStandard $allFiles -o $outputExe 2>&1 | ForEach-Object { Write-Host $_ }
    
    if ($LASTEXITCODE -eq 0) {
        Write-Host ""
        Write-Host "=============================================" -ForegroundColor Green
        Write-Host "  BUILD SUCCESSFUL!" -ForegroundColor Green
        Write-Host "=============================================" -ForegroundColor Green
        Write-Host ""
        Write-Host "Executable created: $outputExe" -ForegroundColor Cyan
        Write-Host ""
        Write-Host "To run the program:" -ForegroundColor Yellow
        Write-Host "  .\$outputExe" -ForegroundColor White
        Write-Host ""
        Write-Host "Or navigate to the build directory:" -ForegroundColor Yellow
        Write-Host "  cd $buildDir" -ForegroundColor White
        Write-Host "  .\smartpark.exe" -ForegroundColor White
        Write-Host ""
    } else {
        Write-Host ""
        Write-Host "=============================================" -ForegroundColor Red
        Write-Host "  BUILD FAILED!" -ForegroundColor Red
        Write-Host "=============================================" -ForegroundColor Red
        Write-Host ""
        Write-Host "Please check the error messages above." -ForegroundColor Yellow
        exit 1
    }
} catch {
    Write-Host ""
    Write-Host "Build error: $_" -ForegroundColor Red
    exit 1
}
