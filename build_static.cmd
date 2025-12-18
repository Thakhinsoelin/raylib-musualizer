@echo off
setlocal

REM ==== CONFIG ====
set GPP=g++
set OUT=build\
REM Path to raylib
set RAYLIB=raylib

REM ==== BUILD ====

%GPP% ^
    -static-libgcc -static-libstdc++ ^
    -I"%RAYLIB%\include" -Isrc\plug\^
    src\plug\*.cpp src\main\*.cpp^
    -L"%RAYLIB%\lib" ^
    -lraylib ^
    -lopengl32 -lgdi32 -lwinmm -mwindows ^
    -o %OUT%musualizer_static

if %errorlevel% neq 0 (
    echo Build failed!
    exit /b 1
)

echo Build successful: %OUT%
endlocal
