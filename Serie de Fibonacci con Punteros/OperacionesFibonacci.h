#ifndef OPERACIONESFIBONACCI_H
#define OPERACIONESFIBONACCI_H

#include <iostream>
#include "Fibonacci.h"
using namespace std;

template <class T>
class OperacionesFibonacci {
public:

    void generar(Fibonacci<T> *f) {
        T n = f->getCantidad();
        T *serie = f->getSerie();

        if (n <= 0) return;

        serie[0] = 0;
        if (n > 1) serie[1] = 1;

        for (T i = 2; i < n; i++) {
            serie[i] = serie[i - 1] + serie[i - 2];
        }
    }

    void mostrarSecuencia(Fibonacci<T> *f) {
        T n = f->getCantidad();
        T *serie = f->getSerie();

        for (T i = 0; i < n; i++) {
            cout << serie[i] << " ";
        }
        cout << endl;
    }

    void mostrarDirecciones(Fibonacci<T> *f) {
        T n = f->getCantidad();
        T *serie = f->getSerie();

        for (T i = 0; i < n; i++) {
            cout << "Pos [" << i << "] -> Valor: " << serie[i]
                 << " | Dir: " << &serie[i] << endl;
        }
    }

    void buscarPosicion(Fibonacci<T> *f, T pos) {
        T n = f->getCantidad();
        T *serie = f->getSerie();

        if (pos >= 0 && pos < n) {
            cout << "Posicion: " << pos << endl;
            cout << "Valor: " << serie[pos] << endl;
            cout << "Direccion: " << &serie[pos] << endl;
        } else {
            cout << "Posicion fuera de rango\n";
        }
    }
};

#endif
