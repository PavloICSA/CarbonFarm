@echo off
echo Farm Carbon Footprint Estimator - Windows Build Script
echo =====================================================

echo.
echo Cleaning up old executables...
taskkill /f /im carbon*.exe 2>nul >nul
timeout /t 1 /nobreak >nul
if exist carbon.exe del /f /q carbon.exe 2>nul

echo.
echo Building unified version with all interfaces (no dependencies)...
gcc src\main.c src\input.c src\compute.c src\report.c src\ui.c src\simple_ui.c -o carbon.exe -lm
if %errorlevel% neq 0 (
    echo ERROR: Failed to build program
    echo This might be due to file permissions or antivirus software.
    echo Try running as administrator or temporarily disable antivirus.
    pause
    exit /b 1
)

echo.
echo +==============================================================+
echo ^|                    Build Complete!                           ^|
echo +==============================================================+
echo.
echo Program built: carbon.exe
echo.
echo Available interfaces:
echo - carbon.exe                    (interactive CLI)
echo - carbon.exe --simple           (simple UI)
echo - carbon.exe --ui               (advanced UI)
echo - carbon.exe data\sample_input.csv (batch processing)
echo - carbon.exe --help             (show help)
echo.
echo All interfaces are dependency-free and work on any Windows system!
echo.
echo Run demo.bat to see all interfaces in action!
pause