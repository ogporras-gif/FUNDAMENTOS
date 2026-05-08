#ifndef LISTA_H
#define LISTA_H

#include "Nodo.h"
#include <fstream>
#include <regex>

class Lista {
private:
    Nodo* cabeza;

public:
    Lista();

    void insertar(Persona persona);

    void mostrar();

    Nodo* buscar(string cedula);

    void eliminar(string cedula);

    bool validarCedula(string cedula);

    bool validarNombre(string nombre);

    bool cedulaRepetida(string cedula);

    bool nombreRepetido(string nombre);

    string leerCedula();

    string leerNombre();

    void guardarArchivo();

    void cargarArchivo();

    string provinciaCedula(string cedula);

    void mostrarContadorProvincias();
};

#endif
