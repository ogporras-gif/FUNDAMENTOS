#ifndef FIBONACCI_H
#define FIBONACCI_H

#include <iostream>
using namespace std;

template <class T>
class Fibonacci {
private:
    T *cantidad;
    T *serie;

public:
    Fibonacci() {
        cantidad = new T(0);
        serie = 0;
    }

    Fibonacci(T c) {
        cantidad = new T(c);
        serie = new T[c];
    }

    Fibonacci(const Fibonacci &f) {
        cantidad = new T(*f.cantidad);
        serie = new T[*cantidad];
        for (int i = 0; i < *cantidad; i++) {
            serie[i] = f.serie[i];
        }
    }

    ~Fibonacci() {
        delete cantidad;
        if (serie != 0)
            delete[] serie;
    }

    T& getCantidad() {
        return *cantidad;
    }

    void setCantidad(T c) {
        *cantidad = c;
        if (serie != 0)
            delete[] serie;
        serie = new T[c];
    }

    T* getSerie() {
        return serie;
    }
};

#endif
