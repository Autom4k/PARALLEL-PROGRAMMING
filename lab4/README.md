# Лабораторные работы по курсу «Параллельное программирование»
# Лабораторная работа 4: Технология CUDA

## Компиляция:
nvcc -O2 main_cuda.cu -o main_cuda.exe

## Запуск:
./gen.exe 200
./main_cuda.exe 16
./main_cuda.exe 32
./main_cuda.exe 64
./main_cuda.exe 128

## Автоматический запуск:
./run_cuda_experiments.bat