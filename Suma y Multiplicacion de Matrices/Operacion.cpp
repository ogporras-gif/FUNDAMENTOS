#include "Operacion.h"
#include <iostream>

using namespace std;
void Operacion::encerar(float** mat, int f, int c) {
    for (int i = 0; i < f; i++) {
        for (int j = 0; j < c; j++) {
            *(*(mat + i) + j) = 0;
        }
    }
}
void Operacion::ingresar(float** mat, int f, int c) {
    for (int i = 0; i < f; i++) {
        for (int j = 0; j < c; j++) {
            cout << "Ingrese [" << i << "][" << j << "]: ";
            cin >> *(*(mat + i) + j);
        }
    }
}

void Operacion::imprimir(float** mat, int f, int c) {
    for (int i = 0; i < f; i++) {
        for (int j = 0; j < c; j++) {
            cout << *(*(mat + i) + j) << "\t";
        }
        cout << endl;
    }
}

void Operacion::sumaRecursiva(float** A, float** B, float** R, int f, int c, int i, int j) {
    if (i >= f) return;

    *(*(R + i) + j) = *(*(A + i) + j) + *(*(B + i) + j);

    if (j < c - 1)
        sumaRecursiva(A, B, R, f, c, i, j + 1);
    else
        sumaRecursiva(A, B, R, f, c, i + 1, 0);
}

void Operacion::multiplicacionRecursiva(float** A, float** B, float** R,
                                        int f1, int c1, int c2,
                                        int i, int j, int k) {

    if (i >= f1) return;

    if (j < c2) {
        if (k < c1) {
            *(*(R + i) + j) += (*(*(A + i) + k)) * (*(*(B + k) + j));
            multiplicacionRecursiva(A, B, R, f1, c1, c2, i, j, k + 1);
        } else {
            multiplicacionRecursiva(A, B, R, f1, c1, c2, i, j + 1, 0);
        }
    } else {
        multiplicacionRecursiva(A, B, R, f1, c1, c2, i + 1, 0, 0);
    }
}
void Operacion::multiplicar(float** A, float** B, float** R, int f1, int c1, int c2) {
    for (int i = 0; i < f1; i++) {
        for (int j = 0; j < c2; j++) {
            *(*(R + i) + j) = 0;
            for (int k = 0; k < c1; k++) {
                *(*(R + i) + j) += (*(*(A + i) + k)) * (*(*(B + k) + j));
            }
        }
    }
}
void Operacion::sumaIterativa(float** A, float** B, float** R, int f, int c) {
    for (int i = 0; i < f; i++) {
        for (int j = 0; j < c; j++) {
            *(*(R + i) + j) = *(*(A + i) + j) + *(*(B + i) + j);
        }
    }
}
