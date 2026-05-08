#ifndef PERSONA_H
#define PERSONA_H

#include <iostream>
using namespace std;

class Persona {
private:
    string nombre;
    string cedula;

public:
    Persona();
    Persona(string n, string c);

    string getNombre();
    string getCedula();

    bool validarCedula();
    string obtenerProvincia();

    void mostrar();
};

#endif
