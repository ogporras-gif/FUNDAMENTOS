/***********************************************************************
 * Module:  Matriz.h
 * Author:  HOME
 * Modified: jueves, 16 de abril de 2026 20:34:24
 * Purpose: Declaration of the class Matriz
 ***********************************************************************/

#if !defined(__MATRIZ_Matriz_h)
#define __MATRIZ_Matriz_h
#include <iostream>
using namespace std;

template <class T>
class Matriz
{
public:
   int getFilas();
   void setFilas(int newFilas);

   int getColumnas();
   void setColumnas(int newColumnas);

   void setDato(int f, int c, T valor);
   T getDato(int f, int c);

   Matriz();
   Matriz(int f, int c);
   ~Matriz();

   void ingresarDatos(void);
   void mostrarMatriz(void);

   Matriz<T> operator+(Matriz<T> &m);
   Matriz<T>& operator=(const Matriz<T>& m);

protected:
private:
   int filas;
   int columnas;
   T **datos;


};
#include "Matriz.cpp";
#endif
