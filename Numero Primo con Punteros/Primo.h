#ifndef PRIMO_H
#define PRIMO_H

#include <iostream>
using namespace std;

template <class T>
class Primo {
private:
    T *numero;

public:
    Primo() {
        numero = new T(0);
    }

    Primo(T n) {
        numero = new T(n);
    }

    Primo(const Primo &p) {
        numero = new T(*p.numero);
    }

    ~Primo() {
        delete numero;
    }

    T& getNumero() {
        return *numero;
    }

    void setNumero(T n) {
        *numero = n;
    }

    void mostrar() {
        cout << *numero;
    }
};

#endif
