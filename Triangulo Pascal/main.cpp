#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <windows.h> 

using namespace std;

vector<long long> triangulo;
int filas = 0;

CRITICAL_SECTION cs;
bool calculando = false;

int ObtenerIndice(int f, int c) {
    return (f * (f + 1)) / 2 + c;
}

void CalcularPascal() {
    int total_elementos = (filas * (filas + 1)) / 2;
    triangulo.assign(total_elementos, 1);

    for (int i = 2; i < filas; i++) {
        for (int j = 1; j < i; j++) {
            int idx_actual = ObtenerIndice(i, j);
            int idx_padre1 = ObtenerIndice(i - 1, j - 1);
            int idx_padre2 = ObtenerIndice(i - 1, j);
            
            triangulo[idx_actual] = triangulo[idx_padre1] + triangulo[idx_padre2];
        }
    }
}

DWORD WINAPI HiloCalculo(LPVOID lpParam) {
    EnterCriticalSection(&cs);
    calculando = true;
    
    CalcularPascal();
    
    calculando = false;
    LeaveCriticalSection(&cs);
    return 0;
}

void MostrarEnConsola() {
    cout << "\n--- TRIANGULO DE PASCAL (" << filas << " FILAS) ---\n\n";
    
    for (int i = 0; i < filas; i++) {
        for (int esp = 0; esp < (filas - i - 1) * 3; esp++) {
            cout << " ";
        }
        for (int j = 0; j <= i; j++) {
            int idx = ObtenerIndice(i, j);
            cout << " " << triangulo[idx] << "    ";
        }
        cout << "\n";
    }
    cout << "\n----------------------------------------\n";
}

int main() {
    time_t tiempo_actual = time(NULL);
    srand(static_cast<unsigned int>(tiempo_actual));
    
    InitializeCriticalSection(&cs);

    cout << "===========================================\n";
    cout << "  GENERADOR DE TRIANGULO DE PASCAL MULTIHILO \n";
    cout << "===========================================\n\n";
    
    cout << "Ingresa el numero de filas que deseas (Recomendado 1-15): ";
    cin >> filas;

    if (filas <= 0) {
        cout << "Error: El numero de filas debe ser mayor a 0.\n";
        DeleteCriticalSection(&cs);
        return 1;
    }
    HANDLE hGilo = CreateThread(NULL, 0, HiloCalculo, NULL, 0, NULL);
    
    if (hGilo != NULL) {
        WaitForSingleObject(hGilo, INFINITE);
        CloseHandle(hGilo);
    }
    EnterCriticalSection(&cs);
    MostrarEnConsola();
    LeaveCriticalSection(&cs);

    DeleteCriticalSection(&cs);
    
    cout << "\nPresiona ENTER para salir...";
    cin.ignore();
    cin.get();
    
    return 0;
}