@echo off
chcp 65001 > nul
echo Эксперименты с MPI
echo ==================

set SIZES=200 400 800 1200 1600 2000
set PROCESSES=1 2 4 8

echo Размер,Процессов,Время(мс),MFLOPS > results_mpi.csv

for %%s in (%SIZES%) do (
    for %%p in (%PROCESSES%) do (
        echo.
        echo [%%sx%%s] Генерация матриц...
        gen.exe %%s
        
        echo [%%sx%%s] Запуск с %%p процессами...
        mpiexec -n %%p main_mpi.exe
        
        timeout /t 2 > nul
    )
)

echo.
echo Эксперименты завершены! Результаты в results_mpi.csv
pause