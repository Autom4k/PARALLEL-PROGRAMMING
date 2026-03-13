#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <windows.h>
#include <omp.h>

using namespace std;
using namespace std::chrono;

vector<vector<double>> readMatrix(const string& filename) {
    ifstream file(filename);
    int n;
    file >> n;
    vector<vector<double>> matrix(n, vector<double>(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            file >> matrix[i][j];
    return matrix;
}

void writeMatrix(const string& filename, const vector<vector<double>>& matrix) {
    ofstream file(filename);
    int n = matrix.size();
    file << n << "\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            file << matrix[i][j] << " ";
        file << "\n";
    }
}

int main(int argc, char* argv[]) {
    SetConsoleOutputCP(65001);
    
    int num_threads = 4;
    if (argc > 1) {
        num_threads = atoi(argv[1]);
    }
    
    omp_set_num_threads(num_threads);
    
    cout << "Чтение матриц..." << endl;
    auto A = readMatrix("matrix_A.txt");
    auto B = readMatrix("matrix_B.txt");
    
    int n = A.size();
    cout << "Размер матриц: " << n << "x" << n << endl;
    cout << "Количество потоков: " << num_threads << endl;
    
    vector<vector<double>> C(n, vector<double>(n, 0.0));
    
    double start_time = omp_get_wtime();
    
    #pragma omp parallel for collapse(2) schedule(static)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double sum = 0.0;
            #pragma omp simd reduction(+:sum)
            for (int k = 0; k < n; k++) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
    
    double end_time = omp_get_wtime();
    double duration = (end_time - start_time) * 1000; 
    
    writeMatrix("matrix_C.txt", C);
    
    long long ops = 2LL * n * n * n;
    double mflops = ops / (duration * 1000.0);
    
    cout << "\n========== РЕЗУЛЬТАТЫ ==========" << endl;
    cout << "Размер матрицы: " << n << "x" << n << endl;
    cout << "Потоков: " << num_threads << endl;
    cout << "Время: " << duration << " мс" << endl;
    cout << "Операций: " << ops << endl;
    cout << "Производительность: " << mflops << " MFLOPS" << endl;
    cout << "=================================" << endl;
    
    cout << "\nCSV: " << n << "," << num_threads << "," << duration << "," << mflops << endl;
    
    return 0;
}