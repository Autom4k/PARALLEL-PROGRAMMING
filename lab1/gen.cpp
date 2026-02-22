#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <windows.h>

using namespace std;

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    int n;
    cout << "Введите размер матриц: ";
    cin >> n;
    
    srand(time(0));
    
    ofstream fa("matrix_A.txt");
    fa << n << "\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            fa << rand() % 10 << " ";
        fa << "\n";
    }
    fa.close();
    
    ofstream fb("matrix_B.txt");
    fb << n << "\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            fb << rand() % 10 << " ";
        fb << "\n";
    }
    fb.close();
    
    cout << "Матрицы " << n << "x" << n << " созданы в файлах matrix_A.txt и matrix_B.txt\n";
    return 0;
}