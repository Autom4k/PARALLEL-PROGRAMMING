#include <iostream>
#include <fstream>
#include <vector>
#include <cuda_runtime.h>
#include <windows.h>

using namespace std;

__global__ void matrixMulKernel(float* A, float* B, float* C, int n) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    
    if (row < n && col < n) {
        float sum = 0.0f;
        for (int k = 0; k < n; k++) {
            sum += A[row * n + k] * B[k * n + col];
        }
        C[row * n + col] = sum;
    }
}

vector<vector<float>> readMatrix(const string& filename) {
    ifstream file(filename);
    int n;
    file >> n;
    vector<vector<float>> matrix(n, vector<float>(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            file >> matrix[i][j];
    return matrix;
}

void writeMatrix(const string& filename, const vector<vector<float>>& matrix) {
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
    
    int blockSize = 16;
    if (argc > 1) {
        blockSize = atoi(argv[1]);
    }
    
    cout << "Чтение матриц..." << endl;
    auto A = readMatrix("matrix_A.txt");
    auto B = readMatrix("matrix_B.txt");
    
    int n = A.size();
    cout << "Размер матриц: " << n << "x" << n << endl;
    cout << "Размер блока: " << blockSize << "x" << blockSize << endl;
    
    float* h_A = new float[n * n];
    float* h_B = new float[n * n];
    float* h_C = new float[n * n]();
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            h_A[i * n + j] = A[i][j];
            h_B[i * n + j] = B[i][j];
        }
    }
    
    float *d_A, *d_B, *d_C;
    cudaMalloc(&d_A, n * n * sizeof(float));
    cudaMalloc(&d_B, n * n * sizeof(float));
    cudaMalloc(&d_C, n * n * sizeof(float));
    
    cudaMemcpy(d_A, h_A, n * n * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, n * n * sizeof(float), cudaMemcpyHostToDevice);
    
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    
    cudaEventRecord(start);
    
    dim3 threadsPerBlock(blockSize, blockSize);
    dim3 numBlocks((n + blockSize - 1) / blockSize, (n + blockSize - 1) / blockSize);
    
    matrixMulKernel<<<numBlocks, threadsPerBlock>>>(d_A, d_B, d_C, n);
    cudaDeviceSynchronize();
    
    cudaEventRecord(stop);
    cudaEventSynchronize(stop);
    
    float duration = 0;
    cudaEventElapsedTime(&duration, start, stop);
    
    cudaMemcpy(h_C, d_C, n * n * sizeof(float), cudaMemcpyDeviceToHost);
    
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);
    
    vector<vector<float>> C(n, vector<float>(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = h_C[i * n + j];
    
    writeMatrix("matrix_C.txt", C);
    
    long long ops = 2LL * n * n * n;
    double gflops = ops / (duration * 1000000.0);
    
    cout << "\n========== РЕЗУЛЬТАТЫ ==========" << endl;
    cout << "Размер матрицы: " << n << "x" << n << endl;
    cout << "Размер блока: " << blockSize << "x" << blockSize << endl;
    cout << "Сетка: " << numBlocks.x << "x" << numBlocks.y << endl;
    cout << "Время: " << duration << " мс" << endl;
    cout << "Операций: " << ops << endl;
    cout << "Производительность: " << gflops << " GFLOPS" << endl;
    cout << "=================================" << endl;
    
    delete[] h_A;
    delete[] h_B;
    delete[] h_C;
    
    return 0;
}