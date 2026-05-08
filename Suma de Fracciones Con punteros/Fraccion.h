#ifndef FRACCION_H
#define FRACCION_H

#include <iostream>
using namespace std;

template <class T>
class Fraccion {
private:
    T *numerador;
    T *denominador;

public:
    Fraccion() {
        numerador = new T(0);
        denominador = new T(1);
    }

    Fraccion(T n, T d) {
        numerador = new T(n);
        if (d != 0)
            denominador = new T(d);
        else {
            denominador = new T(1);
            cout << "Denominador no puede ser 0\n";
        }
    }

    Fraccion(const Fraccion &f) {
        numerador = new T(*f.numerador);
        denominador = new T(*f.denominador);
    }

    ~Fraccion() {
        delete numerador;
        delete denominador;
    }

    T& getNumerador() {
        return *numerador;
    }

    T& getDenominador() {
        return *denominador;
    }

    void setNumerador(T n) {
        *numerador = n;
    }

    void setDenominador(T d) {
        if (d != 0)
            *denominador = d;
        else
            cout << "Error: denominador no puede ser 0\n";
    }

    void mostrar() {
        cout << *numerador << "/" << *denominador;
    }
};

#endif
