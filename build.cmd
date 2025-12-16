@echo off
setlocal

REM ==== CONFIG ====
set GPP=g++
set OUT=build\
set IMPLIB=%OUT%libplug.lib

REM Path to raylib
set RAYLIB=raylib

REM ==== BUILD ====
%GPP% -shared ^
    -I"%RAYLIB%\include" ^
    src\plug\*.cpp ^
    -L"%RAYLIB%\lib" ^
    -lraylib ^
    -lopengl32 -lgdi32 -lwinmm ^
    -Wl,--out-implib,%IMPLIB% ^
    -o %OUT%libplug.dll

%GPP% ^
    -I"%RAYLIB%\include" -Isrc\plug\^
    src\main\*.cpp ^
    -L"%RAYLIB%\lib" ^
    -lraylib ^
    -lopengl32 -lgdi32 -lwinmm ^
    -o %OUT%musualizer

if %errorlevel% neq 0 (
    echo Build failed!
    exit /b 1
)

echo Build successful: %OUT%
endlocal
