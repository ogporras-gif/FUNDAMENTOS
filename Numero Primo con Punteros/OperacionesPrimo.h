#ifndef OPERACIONESPRIMO_H
#define OPERACIONESPRIMO_H

#include "Primo.h"
#include <cmath>

template <class T>
class OperacionesPrimo {
public:

    bool esPrimo(Primo<T> &p) {
        T n = p.getNumero();

        if (n <= 1) return false;

        for (T i = 2; i <= sqrt(n); i++) {
            if (n % i == 0)
                return false;
        }
        return true;
    }
};

#endif
