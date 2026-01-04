@echo off
REM ==========================================
REM Bootstrap compiler build + test runner
REM ==========================================

setlocal

REM Directories
set ROOT=..
set BUILD=%ROOT%\build
set SRC=%ROOT%\bootstrap\examples\hello.t

REM Ensure build directory exists
if not exist "%BUILD%" (
    mkdir "%BUILD%"
)

echo.
echo [1/4] Building bootstrap compiler...
gcc src\*.c -Iinclude -o "%BUILD%\myc0.exe"
IF ERRORLEVEL 1 (
    echo ERROR: Failed to build bootstrap compiler
    exit /b 1
)

echo OK: myc0.exe built

echo.
echo [2/4] Compiling source file: %SRC%
"%BUILD%\myc0.exe" "%SRC%" > "%BUILD%\out.s"
IF ERRORLEVEL 1 (
    echo ERROR: Compilation failed
    exit /b 1
)

echo OK: Assembly generated

echo.
echo [3/4] Assembling and linking...
gcc "%BUILD%\out.s" -o "%BUILD%\out.exe"
IF ERRORLEVEL 1 (
    echo ERROR: Assembly failed
    exit /b 1
)

echo OK: Executable built

echo.
echo [4/4] Running program...
"%BUILD%\out.exe"
set EXITCODE=%ERRORLEVEL%

echo.
echo Program exited with code: %EXITCODE%
echo ==========================================

endlocal
