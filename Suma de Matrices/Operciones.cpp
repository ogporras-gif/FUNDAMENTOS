#include <iostream>
using namespace std;

template <class T>
Matriz<T> Operaciones<T>::sumar(Matriz<T> &a, Matriz<T> &b) {
    int f = a.getFilas();
    int c = a.getColumnas();

    Matriz<T> r(f, c);

    for (int i = 0; i < f; i++) {
        for (int j = 0; j < c; j++) {
            r.setDato(i, j, a.getDato(i, j) + b.getDato(i, j));
        }
    }
    return r;
}
