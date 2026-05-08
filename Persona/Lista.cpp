#include "Lista.h"
#include <iostream>
#include <fstream>

using namespace std;

Lista::Lista() {

    cabeza = NULL;
    cola = NULL;
}

int Lista::contar() {

    int c = 0;

    Nodo* aux = cabeza;

    while (aux != NULL) {

        c++;

        aux = aux->siguiente;
    }

    return c;
}

bool Lista::cedulaExiste(string cedula) {

    Nodo* aux = cabeza;

    while (aux != NULL) {

        if (aux->dato.getCedula() == cedula) {

            return true;
        }

        aux = aux->siguiente;
    }

    return false;
}

void Lista::insertarCabeza(Persona p) {

    Nodo* nuevo = new Nodo(p);

    if (cabeza == NULL) {

        cabeza = cola = nuevo;
    }
    else {

        nuevo->siguiente = cabeza;

        cabeza->anterior = nuevo;

        cabeza = nuevo;
    }
}

void Lista::insertarCola(Persona p) {

    Nodo* nuevo = new Nodo(p);

    if (cola == NULL) {

        cabeza = cola = nuevo;
    }
    else {

        cola->siguiente = nuevo;

        nuevo->anterior = cola;

        cola = nuevo;
    }
}

void Lista::insertarMitad(Persona p) {

    if (cabeza == NULL) {

        insertarCabeza(p);

        return;
    }

    int pos = contar() / 2;

    Nodo* actual = cabeza;

    for (int i = 0; i < pos; i++) {

        actual = actual->siguiente;
    }

    Nodo* nuevo = new Nodo(p);

    Nodo* anterior = actual->anterior;

    if (anterior != NULL) {

        anterior->siguiente = nuevo;

        nuevo->anterior = anterior;
    }

    nuevo->siguiente = actual;

    actual->anterior = nuevo;

    if (actual == cabeza) {

        cabeza = nuevo;
    }
}

void Lista::eliminarCabeza() {

    if (cabeza == NULL)
        return;

    Nodo* temp = cabeza;

    if (cabeza == cola) {

        cabeza = cola = NULL;
    }
    else {

        cabeza = cabeza->siguiente;

        cabeza->anterior = NULL;
    }

    delete temp;
}

void Lista::eliminarCola() {

    if (cola == NULL)
        return;

    Nodo* temp = cola;

    if (cabeza == cola) {

        cabeza = cola = NULL;
    }
    else {

        cola = cola->anterior;

        cola->siguiente = NULL;
    }

    delete temp;
}

void Lista::eliminarPosicion(int pos) {

    Nodo* actual = cabeza;

    for (int i = 0;
         actual != NULL && i < pos;
         i++) {

        actual = actual->siguiente;
    }

    if (actual == NULL)
        return;

    if (actual == cabeza) {

        eliminarCabeza();
    }
    else if (actual == cola) {

        eliminarCola();
    }
    else {

        actual->anterior->siguiente =
            actual->siguiente;

        actual->siguiente->anterior =
            actual->anterior;

        delete actual;
    }
}

void Lista::buscarPorCedula(string cedula) {

    Nodo* aux = cabeza;

    while (aux != NULL) {

        if (aux->dato.getCedula() == cedula) {

            aux->dato.mostrar();

            return;
        }

        aux = aux->siguiente;
    }

    cout << "No encontrado\n";
}

void Lista::mostrarPorProvincia(string provincia) {

    Nodo* aux = cabeza;

    while (aux != NULL) {

        if (aux->dato.obtenerProvincia()
            == provincia) {

            aux->dato.mostrar();
        }

        aux = aux->siguiente;
    }
}

void Lista::mostrarCantidadProvincias() {

    string provincias[24] = {

        "Azuay",
        "Bolivar",
        "Canar",
        "Carchi",
        "Cotopaxi",
        "Chimborazo",
        "El Oro",
        "Esmeraldas",
        "Guayas",
        "Imbabura",
        "Loja",
        "Los Rios",
        "Manabi",
        "Morona Santiago",
        "Napo",
        "Pastaza",
        "Pichincha",
        "Tungurahua",
        "Zamora Chinchipe",
        "Galapagos",
        "Sucumbios",
        "Orellana",
        "Santo Domingo",
        "Santa Elena"
    };

    for (int i = 0; i < 24; i++) {

        int contador = 0;

        Nodo* aux = cabeza;

        while (aux != NULL) {

            if (aux->dato.obtenerProvincia()
                == provincias[i]) {

                contador++;
            }

            aux = aux->siguiente;
        }

        cout << provincias[i]
             << ": "
             << contador
             << endl;
    }
}

void Lista::mostrarAdelante() {

    Nodo* aux = cabeza;

    while (aux != NULL) {

        aux->dato.mostrar();

        aux = aux->siguiente;
    }
}

void Lista::mostrarAtras() {

    Nodo* aux = cola;

    while (aux != NULL) {

        aux->dato.mostrar();

        aux = aux->anterior;
    }
}

void Lista::guardarArchivo() {

    ofstream archivo("personas.txt");

    Nodo* aux = cabeza;

    while (aux != NULL) {

        archivo
            << aux->dato.getNombre()
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

    while (
        getline(archivo, nombre, ';')
    ) {

        getline(archivo, cedula);

        Persona p(nombre, cedula);

        insertarCola(p);
    }

    archivo.close();
}
