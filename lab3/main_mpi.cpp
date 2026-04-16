#include <iostream>
#include <fstream>
#include <vector>
#include <mpi.h>
#include <windows.h>

using namespace std;

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

int main(int argc, char* argv[]) {
    // Инициализация MPI
    MPI_Init(&argc, &argv);
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    double start_time = MPI_Wtime();
    
    int n = 0;
    vector<vector<double>> A, B;
    
    if (rank == 0) {
        cout << "Reading matrices..." << endl;
        A = readMatrix("matrix_A.txt");
        B = readMatrix("matrix_B.txt");
        n = A.size();
        cout << "Matrix size: " << n << "x" << n << endl;
        cout << "Processes: " << size << endl;
    }
    
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    if (n == 0) {
        MPI_Finalize();
        return 1;
    }
    
    vector<double> B_flat(n * n);
    if (rank == 0) {
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                B_flat[i * n + j] = B[i][j];
    }
    
    MPI_Bcast(B_flat.data(), n * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    int rows_per_proc = n / size;
    int remainder = n % size;
    
    vector<int> sendcounts(size), displs(size);
    int offset = 0;
    for (int i = 0; i < size; i++) {
        int rows = rows_per_proc + (i < remainder ? 1 : 0);
        sendcounts[i] = rows * n;
        displs[i] = offset;
        offset += sendcounts[i];
    }
    
    vector<double> A_flat(n * n);
    if (rank == 0) {
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                A_flat[i * n + j] = A[i][j];
    }
    
    int local_rows = rows_per_proc + (rank < remainder ? 1 : 0);
    vector<double> A_local(local_rows * n);
    
    MPI_Scatterv(A_flat.data(), sendcounts.data(), displs.data(), MPI_DOUBLE,
                 A_local.data(), local_rows * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    vector<double> C_local(local_rows * n, 0.0);
    
    for (int i = 0; i < local_rows; i++) {
        for (int j = 0; j < n; j++) {
            double sum = 0.0;
            for (int k = 0; k < n; k++) {
                sum += A_local[i * n + k] * B_flat[k * n + j];
            }
            C_local[i * n + j] = sum;
        }
    }
    
    vector<double> C_flat;
    if (rank == 0) {
        C_flat.resize(n * n);
    }
    
    MPI_Gatherv(C_local.data(), local_rows * n, MPI_DOUBLE,
                C_flat.data(), sendcounts.data(), displs.data(), MPI_DOUBLE,
                0, MPI_COMM_WORLD);
    
    double end_time = MPI_Wtime();
    double duration = (end_time - start_time) * 1000;
    
    if (rank == 0) {
        ofstream file("matrix_C.txt");
        file << n << "\n";
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                file << C_flat[i * n + j] << " ";
            }
            file << "\n";
        }
        file.close();
        
        long long ops = 2LL * n * n * n;
        double mflops = ops / (duration * 1000.0);
        
        cout << "\n========== RESULTS ==========" << endl;
        cout << "Matrix size: " << n << "x" << n << endl;
        cout << "Processes: " << size << endl;
        cout << "Time: " << duration << " ms" << endl;
        cout << "Operations: " << ops << endl;
        cout << "Performance: " << mflops << " MFLOPS" << endl;
        cout << "==============================" << endl;
        
        cout << "CSV: " << n << "," << size << "," << duration << "," << mflops << endl;
    }
    
    MPI_Finalize();
    return 0;
}