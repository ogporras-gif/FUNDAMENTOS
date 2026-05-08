#include <iostream>
#include "Matriz.h"
#include "Operaciones.h"

using namespace std;

int main() {
    int f, c;

    cout << "Filas: ";
    cin >> f;
    cout << "Columnas: ";
    cin >> c;

    Matriz<int> m1(f, c);
    Matriz<int> m2(f, c);

    cout << "Matriz 1:\n";
    m1.ingresarDatos();

    cout << "Matriz 2:\n";
    m2.ingresarDatos();

    Operaciones<int> op;
    Matriz<int> resultado = op.sumar(m1, m2);

    cout << "Resultado:\n";
    resultado.mostrarMatriz();

    return 0;
}
