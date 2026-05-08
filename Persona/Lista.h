#ifndef LISTA_H
#define LISTA_H

#include "Nodo.h"

class Lista {

private:

    Nodo* cabeza;
    Nodo* cola;

public:

    Lista();

    void insertarCabeza(Persona p);
    void insertarCola(Persona p);
    void insertarMitad(Persona p);

    void eliminarCabeza();
    void eliminarCola();
    void eliminarPosicion(int pos);

    void buscarPorCedula(string cedula);

    bool cedulaExiste(string cedula);

    void mostrarPorProvincia(string provincia);

    void mostrarCantidadProvincias();

    void guardarArchivo();
    void cargarArchivo();

    int contar();

    void mostrarAdelante();
    void mostrarAtras();
};

#endif
