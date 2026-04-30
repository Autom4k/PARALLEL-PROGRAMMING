# Лабораторные работы по курсу «Параллельное программирование»
# Лабораторная работа 5: Технология MPI (суперкомпьютер «Сергей Королёв»)

## Компиляция:
mpicxx main_mpi.cpp -o main_mpi

## Запуск:
./gen 200
mpirun -np 1 ./main_mpi
mpirun -np 2 ./main_mpi
mpirun -np 4 ./main_mpi
mpirun -np 8 ./main_mpi

## Автоматический запуск:
./run_mpi_experiments.bat