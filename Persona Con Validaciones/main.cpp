#include "Lista.h"

int main() {

    Lista lista;

    lista.cargarArchivo();

    int opcion;

    string nombre;
    string cedula;

    do {

        cout << "\n===== MENU =====\n";

        cout << "1. Insertar\n";
        cout << "2. Mostrar\n";
        cout << "3. Buscar\n";
        cout << "4. Eliminar\n";
        cout << "5. Provincias\n";
        cout << "6. Salir\n";

        cout << "Opcion: ";
        cin >> opcion;

        cin.ignore();

        switch(opcion) {

            case 1:

                cout << "\nIngrese nombre: ";

                nombre = lista.leerNombre();

                cout << endl;

                if (!lista.validarNombre(nombre)) {

                    cout << "\nNombre invalido\n";

                    break;
                }

                cout << "Ingrese cedula: ";

                cedula = lista.leerCedula();

                cout << endl;

                if (!lista.validarCedula(cedula)) {

                    cout << "\nCedula invalida\n";

                    break;
                }

                lista.insertar(Persona(nombre, cedula));

                cout << "\nPersona guardada\n";

            break;

            case 2:

                cout << endl;

                lista.mostrar();

            break;

            case 3:

                cout << "\nIngrese cedula: ";

                cedula = lista.leerCedula();

                cout << endl;

                if (lista.buscar(cedula) == NULL) {

                    cout << "\nNo encontrada\n";
                }

            break;

            case 4:

                cout << "\nIngrese cedula eliminar: ";

                cedula = lista.leerCedula();

                lista.eliminar(cedula);

                cout << "\nEliminado\n";

            break;

            case 5:

                cout << endl;

                lista.mostrarContadorProvincias();

            break;

            case 6:

                cout << "\nFin\n";

            break;

            default:

                cout << "\nOpcion incorrecta\n";

            break;
        }

    } while(opcion != 6);

    return 0;
}
