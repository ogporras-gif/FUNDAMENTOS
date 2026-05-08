#ifndef OPERACION_H
#define OPERACION_H

#include "Matriz.h"

class Operacion {
public:
    void encerar(float** mat, int f, int c);
    void ingresar(float** mat, int f, int c);
    void imprimir(float** mat, int f, int c);

    void sumaRecursiva(float** A, float** B, float** R, int f, int c, int i = 0, int j = 0);
    void multiplicacionRecursiva(float** A, float** B, float** R, int f1, int c1, int c2, int i = 0, int j = 0, int k = 0);
    void multiplicar(float** A, float** B, float** R, int f1, int c1, int c2);
    void sumaIterativa(float** A, float** B, float** R, int f, int c);
};

#endif
