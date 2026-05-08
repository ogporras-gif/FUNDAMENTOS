/***********************************************************************
 * Module:  Matriz.cpp
 * Author:  HOME
 * Modified: jueves, 16 de abril de 2026 20:34:24
 * Purpose: Implementation of the class Matriz
 ***********************************************************************/

#include "Matriz.h"
#include <iostream>
using namespace std;
////////////////////////////////////////////////////////////////////////
// Name:       Matriz::getFilas()
// Purpose:    Implementation of Matriz::getFilas()
// Return:     int
////////////////////////////////////////////////////////////////////////
template <class T>
int Matriz<T>::getFilas(){
    return filas;
}

////////////////////////////////////////////////////////////////////////
// Name:       Matriz::setFilas(int newFilas)
// Purpose:    Implementation of Matriz::setFilas()
// Parameters:
// - newFilas
// Return:     void
////////////////////////////////////////////////////////////////////////

template <class T>
void Matriz<T>::setFilas(int newFilas)
{
   filas = newFilas;
}

////////////////////////////////////////////////////////////////////////
// Name:       Matriz::getColumnas()
// Purpose:    Implementation of Matriz::getColumnas()
// Return:     int
////////////////////////////////////////////////////////////////////////

template <class T>
int Matriz<T>::getColumnas() {
    return columnas;
}

////////////////////////////////////////////////////////////////////////
// Name:       Matriz::setColumnas(int newColumnas)
// Purpose:    Implementation of Matriz::setColumnas()
// Parameters:
// - newColumnas
// Return:     void
////////////////////////////////////////////////////////////////////////

template <class T>
void Matriz<T>::setColumnas(int newColumnas)
{
   columnas = newColumnas;
}

////////////////////////////////////////////////////////////////////////
// Name:       Matriz::Matriz()
// Purpose:    Implementation of Matriz::Matriz()
// Return:
////////////////////////////////////////////////////////////////////////
template <class T>
Matriz<T>::Matriz()
{
    filas =0;
    columnas =0;
    datos = nullptr;
}
template <class T>
Matriz<T>::Matriz(int f, int c) {
    filas = f;
    columnas = c;

    datos = new T*[filas];
    for (int i = 0; i < filas; i++) {
        datos[i] = new T[columnas];
    }
}

////////////////////////////////////////////////////////////////////////
// Name:       Matriz::~Matriz()
// Purpose:    Implementation of Matriz::~Matriz()
// Return:
////////////////////////////////////////////////////////////////////////

template <class T>
Matriz<T>::~Matriz() {
    for (int i = 0; i < filas; i++) {
        delete[] datos[i];
    }
    delete[] datos;
}

////////////////////////////////////////////////////////////////////////
// Name:       Matriz::ingresarDatos()
// Purpose:    Implementation of Matriz::ingresarDatos()
// Return:     void
////////////////////////////////////////////////////////////////////////

template <class T>
void Matriz<T>::ingresarDatos() {
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            cin >> datos[i][j];
        }
    }
}

////////////////////////////////////////////////////////////////////////
// Name:       Matriz::mostrarMatriz()
// Purpose:    Implementation of Matriz::mostrarMatriz()
// Return:     void
////////////////////////////////////////////////////////////////////////

template <class T>
void Matriz<T>::mostrarMatriz() {
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            cout << datos[i][j] << " ";
        }
        cout << endl;
    }
}

template <class T>
void Matriz<T>::setDato(int f, int c, T valor) {
    datos[f][c] = valor;
}

template <class T>
T Matriz<T>::getDato(int f, int c) {
    return datos[f][c];
}

