@echo off
echo =========================================
echo   Traffic Signal Simulation Pipeline
echo =========================================

echo.
echo 0. Compiling C++ Code...
:: Make sure your C++ file is actually named traffic_controller.cpp!
g++ traffic_controller.cpp -o traffic_controller.exe

echo.
echo 1. Running C++ Fuzzy Logic Engine...
traffic_controller.exe

echo.
echo 2. Generating Python Graphs...
py python.py

echo.
echo Simulation Complete! Check your folder for the PNG graphs.
pause