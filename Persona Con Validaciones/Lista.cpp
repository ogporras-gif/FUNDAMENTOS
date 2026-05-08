#include "Lista.h"
#include <conio.h>

Lista::Lista() {
    cabeza = NULL;
}

void Lista::insertar(Persona persona) {

    Nodo* auxValidar = cabeza;

    while (auxValidar != NULL) {

        if (
            auxValidar->dato.getCedula()
            ==
            persona.getCedula()
           ) {

            cout << "\nEsta persona ya esta registrada\n";

            return;
        }

        auxValidar = auxValidar->siguiente;
    }

    Nodo* nuevo = new Nodo(persona);

    if (cabeza == NULL) {

        cabeza = nuevo;
    }

    else {

        Nodo* aux = cabeza;

        while (aux->siguiente != NULL) {

            aux = aux->siguiente;
        }

        aux->siguiente = nuevo;
    }

    guardarArchivo();

    cout << "\nPersona guardada correctamente\n";
}

void Lista::mostrar() {

    Nodo* aux = cabeza;

    while (aux != NULL) {

        cout << "Nombre: "
             << aux->dato.getNombre()
             << endl;

        cout << "Cedula: "
             << aux->dato.getCedula()
             << endl;

        cout << "Provincia: "
             << provinciaCedula(aux->dato.getCedula())
             << endl;

        cout << endl;

        aux = aux->siguiente;
    }
}

Nodo* Lista::buscar(string cedula) {

    Nodo* aux = cabeza;

    while (aux != NULL) {

        if (aux->dato.getCedula() == cedula) {

            cout << "\nPersona encontrada\n";

            cout << "Nombre: "
                 << aux->dato.getNombre()
                 << endl;

            cout << "Cedula: "
                 << aux->dato.getCedula()
                 << endl;

            cout << "Provincia: "
                 << provinciaCedula(aux->dato.getCedula())
                 << endl;

            return aux;
        }

        aux = aux->siguiente;
    }

    return NULL;
}

void Lista::eliminar(string cedula) {

    if (cabeza == NULL) {
        return;
    }

    if (cabeza->dato.getCedula() == cedula) {

        Nodo* borrar = cabeza;

        cabeza = cabeza->siguiente;

        delete borrar;

        guardarArchivo();

        return;
    }

    Nodo* aux = cabeza;

    while (aux->siguiente != NULL &&
           aux->siguiente->dato.getCedula() != cedula) {

        aux = aux->siguiente;
    }

    if (aux->siguiente != NULL) {

        Nodo* borrar = aux->siguiente;

        aux->siguiente = borrar->siguiente;

        delete borrar;

        guardarArchivo();
    }
}

bool Lista::validarCedula(string cedula) {

    if (cedula.length() != 10) {
        return false;
    }

    int suma = 0;

    int valor;

    for (int i = 0; i < 9; i++) {

        valor = cedula.at(i) - '0';

        if (i % 2 == 0) {

            valor = valor * 2;

            if (valor > 9) {
                valor = valor - 9;
            }
        }

        suma = suma + valor;
    }

    int decena = ((suma / 10) + 1) * 10;

    int digito = decena - suma;

    if (digito == 10) {
        digito = 0;
    }

    return digito == (cedula.at(9) - '0');
}

bool Lista::validarNombre(string nombre) {

    regex patron("^[A-Za-zÁÉÍÓÚáéíóúÑñ ]+$");

    return regex_match(nombre, patron);
}

bool Lista::cedulaRepetida(string cedula) {

    Nodo* aux = cabeza;

    while (aux != NULL) {

        if (aux->dato.getCedula() == cedula) {
            return true;
        }

        aux = aux->siguiente;
    }

    return false;
}
string Lista::leerCedula() {

    string cedula = "";

    char tecla;

    while (true) {

        tecla = getch();

        if (tecla >= '0' && tecla <= '9') {

            if (cedula.length() < 10) {

                cedula = cedula + tecla;

                cout << tecla;
            }
        }

        else if (tecla == 8 && cedula.length() > 0) {

            cedula.erase(cedula.length() - 1, 1);

            cout << "\b \b";
        }

        else if (tecla == 13 && cedula.length() == 10) {

            break;
        }
    }

    return cedula;
}

string Lista::leerNombre() {

    string nombre = "";

    char tecla;

    while (true) {

        tecla = getch();

        if (
            (tecla >= 'A' && tecla <= 'Z')
            ||
            (tecla >= 'a' && tecla <= 'z')
            ||
            tecla == 32
           ) {

            nombre = nombre + tecla;

            cout << tecla;
        }

        else if (tecla == 8 && nombre.length() > 0) {

            nombre.erase(nombre.length() - 1, 1);

            cout << "\b \b";
        }

        else if (tecla == 13 && nombre.length() > 0) {

            break;
        }
    }

    return nombre;
}

void Lista::guardarArchivo() {

    ofstream archivo("personas.txt");

    Nodo* aux = cabeza;

    while (aux != NULL) {

        archivo << aux->dato.getNombre()
                << ";"
                << aux->dato.getCedula()
                << endl;

        aux = aux->siguiente;
    }

    archivo.close();
}

void Lista::cargarArchivo() {

    ifstream archivo("personas.txt");

    string nombre;
    string cedula;

    while (getline(archivo, nombre, ';')) {

        getline(archivo, cedula);

        Persona persona(nombre, cedula);

        Nodo* nuevo = new Nodo(persona);

        if (cabeza == NULL) {
            cabeza = nuevo;
        }

        else {

            Nodo* aux = cabeza;

            while (aux->siguiente != NULL) {
                aux = aux->siguiente;
            }

            aux->siguiente = nuevo;
        }
    }

    archivo.close();
}

string Lista::provinciaCedula(string cedula) {

    string codigo = "";

    codigo = codigo + *(cedula.begin());
    codigo = codigo + *(cedula.begin() + 1);

    if (codigo == "01") return "Azuay";
    if (codigo == "02") return "Bolivar";
    if (codigo == "03") return "Canar";
    if (codigo == "04") return "Carchi";
    if (codigo == "05") return "Cotopaxi";
    if (codigo == "06") return "Chimborazo";
    if (codigo == "07") return "El Oro";
    if (codigo == "08") return "Esmeraldas";
    if (codigo == "09") return "Guayas";
    if (codigo == "10") return "Imbabura";
    if (codigo == "11") return "Loja";
    if (codigo == "12") return "Los Rios";
    if (codigo == "13") return "Manabi";
    if (codigo == "14") return "Morona Santiago";
    if (codigo == "15") return "Napo";
    if (codigo == "16") return "Pastaza";
    if (codigo == "17") return "Pichincha";
    if (codigo == "18") return "Tungurahua";
    if (codigo == "19") return "Zamora Chinchipe";
    if (codigo == "20") return "Galapagos";
    if (codigo == "21") return "Sucumbios";
    if (codigo == "22") return "Orellana";
    if (codigo == "23") return "Santo Domingo";
    if (codigo == "24") return "Santa Elena";

    return "Desconocida";
}

void Lista::mostrarContadorProvincias() {

    int* contadores = new int(25);

    int i = 0;

    while (i < 25) {

        *(contadores + i) = 0;

        i++;
    }

    Nodo* aux = cabeza;

    while (aux != NULL) {

        string ced = aux->dato.getCedula();

        int codigo;

        codigo =
        ((*(ced.begin()) - '0') * 10)
        +
        (*(ced.begin() + 1) - '0');

        if (codigo >= 1 && codigo <= 24) {

            *(contadores + codigo) =
            *(contadores + codigo) + 1;
        }

        aux = aux->siguiente;
    }

    cout << "\n===== PROVINCIAS =====\n\n";

    cout << "Azuay: " << *(contadores + 1) << endl;
    cout << "Bolivar: " << *(contadores + 2) << endl;
    cout << "Canar: " << *(contadores + 3) << endl;
    cout << "Carchi: " << *(contadores + 4) << endl;
    cout << "Cotopaxi: " << *(contadores + 5) << endl;
    cout << "Chimborazo: " << *(contadores + 6) << endl;
    cout << "El Oro: " << *(contadores + 7) << endl;
    cout << "Esmeraldas: " << *(contadores + 8) << endl;
    cout << "Guayas: " << *(contadores + 9) << endl;
    cout << "Imbabura: " << *(contadores + 10) << endl;
    cout << "Loja: " << *(contadores + 11) << endl;
    cout << "Los Rios: " << *(contadores + 12) << endl;
    cout << "Manabi: " << *(contadores + 13) << endl;
    cout << "Morona Santiago: " << *(contadores + 14) << endl;
    cout << "Napo: " << *(contadores + 15) << endl;
    cout << "Pastaza: " << *(contadores + 16) << endl;
    cout << "Pichincha: " << *(contadores + 17) << endl;
    cout << "Tungurahua: " << *(contadores + 18) << endl;
    cout << "Zamora Chinchipe: " << *(contadores + 19) << endl;
    cout << "Galapagos: " << *(contadores + 20) << endl;
    cout << "Sucumbios: " << *(contadores + 21) << endl;
    cout << "Orellana: " << *(contadores + 22) << endl;
    cout << "Santo Domingo: " << *(contadores + 23) << endl;
    cout << "Santa Elena: " << *(contadores + 24) << endl;

    delete contadores;
}
