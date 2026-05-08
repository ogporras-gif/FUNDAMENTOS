#include "Matriz.h"
#include <stdlib.h>

Matriz::Matriz(int f, int c) {
    filas = f;
    columnas = c;

    mat = (float**)malloc(filas * sizeof(float*));
    for (int i = 0; i < filas; i++) {
        mat[i] = (float*)malloc(columnas * sizeof(float));
    }
}

Matriz::~Matriz() {
    for (int i = 0; i < filas; i++) {
        free(mat[i]);
    }
    free(mat);
}

float** Matriz::getMat() {
    return mat;
}

int Matriz::getFilas() {
    return filas;
}

int Matriz::getColumnas() {
    return columnas;
}
