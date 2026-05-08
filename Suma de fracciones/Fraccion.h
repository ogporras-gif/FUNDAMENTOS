#ifndef FRACCION_H
#define FRACCION_H

#include <iostream>
using namespace std;

template <class T>
class Fraccion {
private:
    T numerador;
    T denominador;

public:
    Fraccion(T n = 0, T d = 1) {
        numerador = n;
        if (d != 0)
            denominador = d;
        else {
            denominador = 1;
            cout << "Denominador no puede ser 0, se asigno 1\n";
        }
    }

    ~Fraccion() {}
    T getNumerador() {
        return numerador;
    }

    T getDenominador() {
        return denominador;
    }
    void setNumerador(T n) {
        numerador = n;
    }

    void setDenominador(T d) {
        if (d != 0)
            denominador = d;
        else
            cout << "Error: denominador no puede ser 0\n";
    }
    void mostrar() {
        cout << numerador << "/" << denominador;
    }
};

#endif
