#include <iostream>
#include "Fraccion.h"
#include "Operaciones.h"

using namespace std;

int main() {
    int opcion;

    Fraccion<float> *f1 = new Fraccion<float>();
    Fraccion<float> *f2 = new Fraccion<float>();
    Fraccion<float> resultado;

    Operaciones<float> op;

    do {
        cout << "\n===== MENU =====\n";
        cout << "1. Ingresar Fraccion 1\n";
        cout << "2. Ingresar Fraccion 2\n";
        cout << "3. Mostrar Fracciones\n";
        cout << "4. Sumar\n";
        cout << "5. Mostrar direcciones\n";
        cout << "6. Mostrar valores\n";
        cout << "7. Salir\n";
        cout << "Opcion: ";
        cin >> opcion;

        switch(opcion) {
            case 1: {
                float n, d;
                cout << "Numerador 1: "; cin >> n;
                cout << "Denominador 1: "; cin >> d;
                f1->setNumerador(n);
                f1->setDenominador(d);
                break;
            }
            case 2: {
                float n, d;
                cout << "Numerador 2: "; cin >> n;
                cout << "Denominador 2: "; cin >> d;
                f2->setNumerador(n);
                f2->setDenominador(d);
                break;
            }
            case 3: {
                cout << "\nFraccion 1: "; f1->mostrar();
                cout << "\nFraccion 2: "; f2->mostrar();
                cout << endl;
                break;
            }
            case 4: {
                resultado = op.sumar(*f1, *f2);
                cout << "\nResultado: ";
                resultado.mostrar();
                cout << endl;
                break;
            }
            case 5: {
                cout << "\n--- Direcciones ---\n";
                cout << "Objeto f1: " << f1 << endl;
                cout << "Numerador f1: " << &f1->getNumerador() << endl;
                cout << "Denominador f1: " << &f1->getDenominador() << endl;

                cout << "\nObjeto f2: " << f2 << endl;
                cout << "Numerador f2: " << &f2->getNumerador() << endl;
                cout << "Denominador f2: " << &f2->getDenominador() << endl;
                break;
            }
            case 6: {
                cout << "\n--- Valores ---\n";
                cout << "f1: " << f1->getNumerador() << "/" << f1->getDenominador() << endl;
                cout << "f2: " << f2->getNumerador() << "/" << f2->getDenominador() << endl;
                break;
            }
            case 7:
                cout << "Saliendo...\n";
                break;
            default:
                cout << "Opcion no valida\n";
        }

    } while(opcion != 7);

    delete f1;
    delete f2;

    return 0;
}
