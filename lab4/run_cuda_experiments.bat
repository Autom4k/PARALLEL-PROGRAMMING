@echo off
chcp 65001 > nul
echo Эксперименты с CUDA
echo ===================

set SIZES=200 400 800 1200 1600 2000
set BLOCKS=16 32 64 128

echo Размер,Блок,Время(мс),GFLOPS > results_cuda.csv

for %%s in (%SIZES%) do (
    for %%b in (%BLOCKS%) do (
        echo.
        echo [%%sx%%s] Генерация матриц...
        gen.exe %%s
        
        echo [%%sx%%s] Размер блока: %%b
        main_cuda.exe %%b
        
        timeout /t 2 > nul
    )
)

echo.
echo Эксперименты завершены! Результаты в results_cuda.csv
pause