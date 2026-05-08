#include <iostream>
#include "Matriz.h"
#include "Operacion.h"

using namespace std;

int main() {
    int f, c;

    cout << "Filas: ";
    cin >> f;
    cout << "Columnas: ";
    cin >> c;

    Matriz A(f, c);
    Matriz B(f, c);
    Matriz R(f, c);

    Operacion op;

    cout << "\n--- MATRIZ A ---\n";
    op.ingresar(A.getMat(), f, c);

    cout << "\n--- MATRIZ B ---\n";
    op.ingresar(B.getMat(), f, c);

    op.sumaRecursiva(A.getMat(), B.getMat(), R.getMat(), f, c);

    cout << "\n--- SUMA (RECURSIVA) ---\n";
    op.imprimir(R.getMat(), f, c);

    op.sumaIterativa(A.getMat(), B.getMat(), R.getMat(), f, c);

    cout << "\n--- SUMA (ITERATIVA) ---\n";
    op.imprimir(R.getMat(), f, c);

    Matriz M(f, c);

    op.encerar(M.getMat(), f, c);

    op.multiplicacionRecursiva(A.getMat(), B.getMat(), M.getMat(), f, c, c);

    cout << "\n--- MULTIPLICACION (RECURSIVA) ---\n";
    op.imprimir(M.getMat(), f, c);

    op.encerar(M.getMat(), f, c);

    op.multiplicar(A.getMat(), B.getMat(), M.getMat(), f, c, c);

    cout << "\n--- MULTIPLICACION (ITERATIVA) ---\n";
    op.imprimir(M.getMat(), f, c);

    return 0;
}
