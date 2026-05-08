#include <iostream>
#include "Fibonacci.h"
#include "OperacionesFibonacci.h"

using namespace std;

int main() {
    int opcion;
    Fibonacci<int> fib;
    OperacionesFibonacci<int> op;

    do {
        cout << "\n===== MENU FIBONACCI =====\n";
        cout << "1. Ingresar cantidad\n";
        cout << "2. Mostrar secuencia\n";
        cout << "3. Mostrar un termino\n";
        cout << "4. Salir\n";
        cout << "Opcion: ";
        cin >> opcion;

        switch(opcion) {
            case 1: {
                int c;
                cout << "Cantidad: ";
                cin >> c;
                fib.setCantidad(c);
                break;
            }
            case 2: {
                cout << "Secuencia: ";
                op.mostrarSecuencia(fib);
                break;
            }
            case 3: {
                int n;
                cout << "Posicion: ";
                cin >> n;
                cout << "Resultado: " << op.obtenerTermino(n) << endl;
                break;
            }
            case 4:
                cout << "Saliendo...\n";
                break;
            default:
                cout << "Opcion no valida\n";
        }

    } while(opcion != 4);

    return 0;
}
