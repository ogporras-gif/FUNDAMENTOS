#include <iostream>
#include "Fibonacci.h"
#include "OperacionesFibonacci.h"

using namespace std;

int main() {
    int opcion;

    Fibonacci<int> *fib = new Fibonacci<int>();
    OperacionesFibonacci<int> op;

    do {
        cout << "\n===== MENU FIBONACCI =====\n";
        cout << "1. Ingresar cantidad a mostrar\n";
        cout << "2. Mostrar secuencia\n";
        cout << "3. Mostrar un termino (valor + direccion)\n";
        cout << "4. Mostrar direcciones completas\n";
        cout << "5. Mostrar valores\n";
        cout << "6. Salir\n";
        cout << "Opcion: ";
        cin >> opcion;

        switch(opcion) {
            case 1: {
                int c;
                cout << "Cantidad: ";
                cin >> c;
                fib->setCantidad(c);
                op.generar(fib);
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
                op.buscarPosicion(fib, n);
                break;
            }
            case 4: {
                cout << "\n--- Direcciones ---\n";
                op.mostrarDirecciones(fib);
                break;
            }
            case 5: {
                cout << "\n--- Valores ---\n";
                cout << "Cantidad almacenada: " << fib->getCantidad() << endl;
                break;
            }
            case 6:
                cout << "Saliendo...\n";
                break;
            default:
                cout << "Opcion no valida\n";
        }

    } while(opcion != 6);

    delete fib;

    return 0;
}
