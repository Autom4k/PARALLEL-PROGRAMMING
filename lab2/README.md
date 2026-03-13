# Лабораторные работы по курсу «Параллельное программирование»
# Лабораторная работа 2: Технология OpenMP

## Компиляция:
g++ gen.cpp -o gen.exe
g++ -fopenmp -O2 main_omp.cpp -o main_omp.exe

## Запуск:
./gen.exe
./main_omp.exe
