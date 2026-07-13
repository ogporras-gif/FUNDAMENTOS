#ifndef NREINAS_H
#define NREINAS_H

#include <iostream>
#include <fstream>

using namespace std;

class NReinas
{
private:

    int **tablero;
    int n;
    int soluciones;
    ofstream archivo;

public:

    NReinas(int);

    ~NReinas();

    bool esSeguro(int,int);

    void imprimir();

    void resolver(int);

    void iniciar();

};

#endif
