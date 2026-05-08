#ifndef MATRIZ_H
#define MATRIZ_H

class Matriz {
private:
    int filas;
    int columnas;
    float** mat;

public:
    Matriz(int f, int c);
    ~Matriz();

    float** getMat();
    int getFilas();
    int getColumnas();
};

#endif
