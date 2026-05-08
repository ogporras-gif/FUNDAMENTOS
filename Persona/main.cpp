#include <iostream>
#include "Lista.h"

using namespace std;

int main() {

    Lista lista;

    lista.cargarArchivo();

    int op;
    int pos;

    string nombre;
    string cedula;
    string provincia;

    do {

        cout << "\n========= MENU =========";
        cout << "\n1 Insertar cabeza";
        cout << "\n2 Insertar cola";
        cout << "\n3 Insertar mitad";
        cout << "\n4 Mostrar adelante";
        cout << "\n5 Mostrar atras";
        cout << "\n6 Buscar cedula";
        cout << "\n7 Eliminar cabeza";
        cout << "\n8 Eliminar cola";
        cout << "\n9 Eliminar posicion";
        cout << "\n10 Mostrar provincia";
        cout << "\n11 Cantidad por provincias";
        cout << "\n12 Salir";
        cout << "\nOpcion: ";

        cin >> op;

        switch(op) {

        case 1: {

            cout << "Nombre: ";
            cin >> nombre;

            cout << "Cedula: ";
            cin >> cedula;

            Persona p(nombre, cedula);

            if (!p.validarCedula()) {

                cout << "Cedula invalida\n";

                break;
            }

            if (lista.cedulaExiste(cedula)) {

                cout << "Cedula repetida\n";

                break;
            }

            lista.insertarCabeza(p);

            lista.guardarArchivo();

            cout << "Insertado por cabeza\n";

            break;
        }

        case 2: {

            cout << "Nombre: ";
            cin >> nombre;

            cout << "Cedula: ";
            cin >> cedula;

            Persona p(nombre, cedula);

            if (!p.validarCedula()) {

                cout << "Cedula invalida\n";

                break;
            }

            if (lista.cedulaExiste(cedula)) {

                cout << "Cedula repetida\n";

                break;
            }

            lista.insertarCola(p);

            lista.guardarArchivo();

            cout << "Insertado por cola\n";

            break;
        }

        case 3: {

            cout << "Nombre: ";
            cin >> nombre;

            cout << "Cedula: ";
            cin >> cedula;

            Persona p(nombre, cedula);

            if (!p.validarCedula()) {

                cout << "Cedula invalida\n";

                break;
            }

            if (lista.cedulaExiste(cedula)) {

                cout << "Cedula repetida\n";

                break;
            }

            lista.insertarMitad(p);

            lista.guardarArchivo();

            cout << "Insertado en la mitad\n";

            break;
        }

        case 4: {

            lista.mostrarAdelante();

            break;
        }

        case 5: {

            lista.mostrarAtras();

            break;
        }

        case 6: {

            cout << "Cedula: ";
            cin >> cedula;

            lista.buscarPorCedula(cedula);

            break;
        }

        case 7: {

            lista.eliminarCabeza();

            lista.guardarArchivo();

            cout << "Cabeza eliminada\n";

            break;
        }

        case 8: {

            lista.eliminarCola();

            lista.guardarArchivo();

            cout << "Cola eliminada\n";

            break;
        }

        case 9: {

            cout << "Posicion: ";
            cin >> pos;

            lista.eliminarPosicion(pos);

            lista.guardarArchivo();

            cout << "Posicion eliminada\n";

            break;
        }

        case 10: {

            cin.ignore();

            cout << "Provincia: ";

            getline(cin, provincia);

            lista.mostrarPorProvincia(provincia);

            break;
        }

        case 11: {

            lista.mostrarCantidadProvincias();

            break;
        }

        case 12: {

            cout << "Saliendo...\n";

            break;
        }

        default: {

            cout << "Opcion invalida\n";
        }

        }

    } while(op != 12);

    return 0;
}
