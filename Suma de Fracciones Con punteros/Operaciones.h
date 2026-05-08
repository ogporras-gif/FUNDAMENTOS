#ifndef OPERACIONES_H
#define OPERACIONES_H

#include "Fraccion.h"

template <class T>
class Operaciones {
public:
    Fraccion<T> sumar(Fraccion<T> &f1, Fraccion<T> &f2) {
        T num = f1.getNumerador() * f2.getDenominador() +
                f2.getNumerador() * f1.getDenominador();

        T den = f1.getDenominador() * f2.getDenominador();

        return Fraccion<T>(num, den);
    }
};

#endif
