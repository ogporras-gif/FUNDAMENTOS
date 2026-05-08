#include <iostream>
#include "Primo.h"
#include "OperacionesPrimo.h"

using namespace std;

int main() {
    int opcion;

    Primo<int> *p1 = new Primo<int>();
    Primo<int> *p2 = new Primo<int>();
    OperacionesPrimo<int> op;

    do {
        cout << "\n===== MENU NUMEROS PRIMOS =====\n";
        cout << "1. Ingresar numero 1\n";
        cout << "2. Ingresar numero 2\n";
        cout << "3. Mostrar numeros\n";
        cout << "4. Verificar si los numero son primos\n";
        cout << "5. Mostrar direccion\n";
        cout << "6. Mostrar valores\n";
        cout << "7. Salir\n";
        cout << "Opcion: ";
        cin >> opcion;

        switch(opcion) {
            case 1: {
                int n;
                cout << "Ingrese numero: ";
                cin >> n;
                p1->setNumero(n);
                break;
            }
            case 2: {
                int n;
                cout << "Ingrese numero: ";
                cin >> n;
                p2->setNumero(n);
                break;
            }
            case 3: {
                cout << "Numero 1: ";
                p1->mostrar();
                cout << endl;
                cout << "\nNumero 2: ";
                p2->mostrar();
                cout << endl;
                break;
            }
            case 4: {
                if (op.esPrimo(*p1))
                    cout << "El primer numero es primo\n";
                else
                    cout << "El primer numero no es primo\n";
                if (op.esPrimo(*p2))
                    cout << "El segundo numero es primo\n";
                else
                    cout << "El segundo numero no es primo\n";
                break;
            }
            case 5: {
                cout << "Direccion del objeto 1: " << p1 << endl;
                cout << "Direccion del numero 1: " << &p1->getNumero() << endl;
                cout << "Direccion del objeto 2: " << p2 << endl;
                cout << "Direccion del numero 2: " << &p2->getNumero() << endl;
                break;
            }
            case 6: {
                cout << "Valor almacenado 1: " << p1->getNumero() << endl;
                cout << "Valor almacenado 2: " << p2->getNumero() << endl;
                break;
            }
            case 7:
                cout << "Saliendo...\n";
                break;
            default:
                cout << "Opcion no valida\n";
        }

    } while(opcion != 7);

    delete p1;
    delete p2;

    return 0;
}
