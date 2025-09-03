@echo off
setlocal enabledelayedexpansion

echo +==============================================================+
echo ^|          Farm Carbon Footprint Estimator Demo               ^|
echo +==============================================================+
echo.

REM Check if program exists, build if needed
if not exist "carbon.exe" (
    echo Building program...
    call build.bat
    if errorlevel 1 (
        echo ERROR: Failed to build program
        pause
        exit /b 1
    )
    echo [OK] Build successful
    echo.
)

:menu
cls
echo +==============================================================+
echo ^|          Farm Carbon Footprint Estimator Demo               ^|
echo +==============================================================+
echo ^|                                                              ^|
echo ^|  Available Demo Options:                                     ^|
echo ^|                                                              ^|
echo ^|  1. NEW: Startup Menu Demo (new interactive menu system)    ^|
echo ^|  2. CLI Demo (batch processing with sample data)            ^|
echo ^|  3. CLI Interactive Demo (command-line prompts)             ^|
echo ^|  4. Simple UI Demo (basic interface)                        ^|
echo ^|  5. Advanced UI Demo (interactive interface)                ^|
echo ^|  6. Compare All Outputs (run sample data through all)       ^|
echo ^|  7. Help & Information                                       ^|
echo ^|  8. Exit                                                     ^|
echo ^|                                                              ^|
echo +==============================================================+
echo.
set /p choice="Enter your choice (1-8): "

if "%choice%"=="1" goto startup_menu_demo
if "%choice%"=="2" goto cli_batch
if "%choice%"=="3" goto cli_interactive
if "%choice%"=="4" goto simple_ui
if "%choice%"=="5" goto advanced_ui
if "%choice%"=="6" goto compare_all
if "%choice%"=="7" goto help
if "%choice%"=="8" goto exit
echo Invalid choice. Please try again.
pause
goto menu

:startup_menu_demo
cls
echo +==============================================================+
echo ^|                NEW: Startup Menu Demo                        ^|
echo +==============================================================+
echo.
echo This demonstrates the NEW startup menu system!
echo.
echo When you run carbon.exe without arguments, you now see:
echo   ========================================
echo       Farm Carbon Footprint Estimator
echo   ========================================
echo   Select Mode of Operation:
echo     1. CLI Mode (Multi-Crop Support, interactive inputs)
echo     2. Simple UI Mode (Multi-Crop or Legacy choice)
echo     3. Advanced UI Mode (Legacy Single-Crop)
echo     4. Batch Mode (load from CSV file)
echo     5. Help ^& Exit
echo   ========================================
echo.
echo Let's test the Help option (choice 5):
echo.
echo 5 | carbon.exe
echo.
echo [OK] This is the new default behavior - no command-line flags needed!
echo The program returns to the menu after each operation for better UX.
pause
goto menu

:cli_batch
cls
echo +==============================================================+
echo ^|                    CLI Batch Demo                            ^|
echo +==============================================================+
echo.
echo Running CLI version with sample data from data\sample_input.csv
echo.
carbon.exe data\sample_input.csv
echo.
echo [OK] Demo completed! Report saved to report.txt
pause
goto menu

:cli_interactive
cls
echo +==============================================================+
echo ^|                 CLI Interactive Demo                         ^|
echo +==============================================================+
echo.
echo Starting CLI interactive mode...
echo You'll be prompted to enter farm data step by step.
echo.
pause
carbon.exe
pause
goto menu

:simple_ui
cls
echo +==============================================================+
echo ^|                   Simple UI Demo                             ^|
echo +==============================================================+
echo.
echo Starting Simple UI with basic menu-driven interface...
echo Use number keys to navigate menus.
echo.
pause
carbon.exe --simple
pause
goto menu

:advanced_ui
cls
echo +==============================================================+
echo ^|                 Advanced UI Demo                             ^|
echo +==============================================================+
echo.
echo Starting Advanced UI with interactive console interface...
echo Use arrow keys to navigate, Enter to select/edit.
echo No external dependencies required!
echo.
pause
carbon.exe --ui
pause
goto menu

:compare_all
cls
echo +==============================================================+
echo ^|                    Compare All Outputs                       ^|
echo +==============================================================+
echo.
echo Running the same sample data through all available interfaces...
echo.

echo 1. CLI Batch Processing:
echo ----------------------------------------
carbon.exe data\sample_input.csv
echo.

echo 2. Simple UI Batch Processing:
echo ----------------------------------------
carbon.exe --simple data\sample_input.csv
echo.

echo 3. Advanced UI Batch Processing:
echo ----------------------------------------
carbon.exe --ui data\sample_input.csv
echo.

echo [OK] All outputs should be identical - only the interface differs!
echo Check report.txt for the saved results.
pause
goto menu

:help
cls
echo +==============================================================+
echo ^|                    Help & Information                        ^|
echo +==============================================================+
echo.
echo AVAILABLE MODES:
echo.
echo CLI Mode (default)
echo   • Traditional CLI with prompts
echo   • Perfect for batch processing and automation
echo   • Usage: carbon.exe [csv_file]
echo.
echo Simple UI Mode (--simple)
echo   • Basic menu-driven console interface
echo   • Text-based prompts and forms
echo   • No external dependencies required
echo   • Maximum compatibility across systems
echo.
echo Advanced UI Mode (--ui)
echo   • Interactive console interface with navigation
echo   • Box drawing characters and visual feedback
echo   • Arrow key navigation and real-time editing
echo   • No external dependencies required
echo   • Recommended for most users
echo.
echo SAMPLE DATA:
echo   • data\sample_input.csv contains example farm data
echo   • 50 hectare wheat farm with typical inputs
echo   • Results in ~2.77 tCO₂e/ha (moderate emissions)
echo.
echo FILES CREATED:
echo   • report.txt - Detailed emission report
echo   • Automatically saved after calculations
echo.
pause
goto menu

:exit
cls
echo +==============================================================+
echo ^|                         Thank You!                           ^|
echo +==============================================================+
echo.
echo Thank you for trying the Farm Carbon Footprint Estimator!
echo.
echo Key Features Demonstrated:
echo [OK] Multiple interface options (CLI, Simple UI, Advanced UI)
echo [OK] Batch processing with CSV files
echo [OK] Interactive data entry
echo [OK] Scientific emission calculations
echo [OK] Actionable sustainability recommendations
echo [OK] Professional report generation
echo.
echo NEW Usage (Startup Menu System):
echo - carbon.exe                    (shows startup menu with 5 options)
echo.
echo Legacy Usage (Still Supported):
echo - carbon.exe data\sample_input.csv (direct batch processing)
echo - carbon.exe --simple           (simple UI)
echo - carbon.exe --ui               (advanced UI)
echo - carbon.exe --help             (show help)
echo.
echo The startup menu makes the program more user-friendly!
echo No need to remember command-line flags anymore.
echo.
echo Happy sustainable farming!
echo.
pause
exit /b 0