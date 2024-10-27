@echo off
@REM cls

cmake --build --preset default && .\build\Release\rocket-ui.exe
