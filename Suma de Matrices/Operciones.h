/***********************************************************************
 * Module:  Matriz.h
 * Author:  HOME
 * Modified: jueves, 16 de abril de 2026 20:34:24
 * Purpose: Declaration of the class Operaciones
 ***********************************************************************/
#ifndef !defined(__MATRIZ_Operaciones_h)
#define __MATRIZ_Operaciones_h

#include "Matriz.h"

template <class T>
class Operaciones {
public:
    Matriz<T> sumar(Matriz<T> &a, Matriz<T> &b);
};

#include "Operaciones.cpp"
#endif
