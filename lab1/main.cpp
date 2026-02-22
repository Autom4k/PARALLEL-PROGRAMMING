#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <windows.h>

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

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    auto A = readMatrix("matrix_A.txt");
    
    auto B = readMatrix("matrix_B.txt");
    
    int n = A.size();
    cout << "Размер матриц: " << n << "x" << n << endl;
    
    vector<vector<double>> C(n, vector<double>(n, 0.0));
    
    auto start = high_resolution_clock::now();
    
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++)
                C[i][j] += A[i][k] * B[k][j];
    
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    
    writeMatrix("matrix_C.txt", C);
    
    cout << "\n========== РЕЗУЛЬТАТЫ ==========" << endl;
    cout << "Размер матрицы: " << n << "x" << n << endl;
    cout << "Количество операций: " << 2LL * n * n * n << endl;
    cout << "Время выполнения: " << duration.count() << " мс" << endl;
    cout << "=================================" << endl;
    
    return 0;
}