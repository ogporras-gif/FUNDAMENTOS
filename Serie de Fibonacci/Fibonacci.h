#ifndef FIBONACCI_H
#define FIBONACCI_H

#include <iostream>
using namespace std;

template <class T>
class Fibonacci {
private:
    T cantidad;

public:
    Fibonacci() {
        cantidad = 0;
    }

    Fibonacci(T c) {
        cantidad = c;
    }

    Fibonacci(const Fibonacci &f) {
        cantidad = f.cantidad;
    }

    ~Fibonacci() {}

    T getCantidad() {
        return cantidad;
    }

    void setCantidad(T c) {
        cantidad = c;
    }
};

#endif
