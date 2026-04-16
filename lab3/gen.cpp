#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <windows.h>

using namespace std;

int main(int argc, char* argv[]) {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    int n;
    if (argc > 1) {
        n = atoi(argv[1]);
        cout << "Генерация матриц " << n << "x" << n << "..." << endl;
    } else {
        cout << "Введите размер матриц: ";
        cin >> n;
    }
    
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
    
    return 0;
}