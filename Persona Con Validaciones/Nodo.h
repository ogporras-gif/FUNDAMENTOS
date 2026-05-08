#ifndef NODO_H
#define NODO_H

#include "Persona.h"

class Nodo {
public:
    Persona dato;
    Nodo* siguiente;

    Nodo(Persona dato);
};

#endif
