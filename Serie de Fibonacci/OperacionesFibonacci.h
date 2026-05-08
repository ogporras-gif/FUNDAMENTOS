#ifndef OPERACIONESFIBONACCI_H
#define OPERACIONESFIBONACCI_H

#include <iostream>
#include "Fibonacci.h"
using namespace std;

template <class T>
class OperacionesFibonacci {
public:

    void mostrarSecuencia(Fibonacci<T> f) {
        T n = f.getCantidad();
        T a = 0, b = 1, siguiente;

        for (T i = 0; i < n; i++) {
            cout << a << " ";
            siguiente = a + b;
            a = b;
            b = siguiente;
        }
        cout << endl;
    }

    T obtenerTermino(T n) {
        T a = 0, b = 1, siguiente;

        if (n == 0) return 0;
        if (n == 1) return 1;

        for (T i = 2; i <= n; i++) {
            siguiente = a + b;
            a = b;
            b = siguiente;
        }

        return b;
    }
};

#endif
