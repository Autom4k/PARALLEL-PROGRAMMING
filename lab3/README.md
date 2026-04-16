# Лабораторные работы по курсу «Параллельное программирование»
# Лабораторная работа 3: Технология MPI

## Компиляция:
g++ gen.cpp -o gen.exe
g++ -O2 main_mpi.cpp -o main_mpi.exe -I"C:\Program Files (x86)\Microsoft SDKs\MPI\Include" -L"C:\Program Files (x86)\Microsoft SDKs\MPI\Lib\x64" -lmsmpi

## Запуск:
./gen.exe 200
mpiexec -n 1 ./main_mpi.exe
mpiexec -n 2 ./main_mpi.exe
mpiexec -n 4 ./main_mpi.exe
mpiexec -n 8 ./main_mpi.exe

## Автоматический запуск:
./run_mpi_experiments.bat