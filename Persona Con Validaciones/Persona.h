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
    Persona(string nombre, string cedula);

    string getNombre();
    string getCedula();

    void setNombre(string nombre);
    void setCedula(string cedula);
};

#endif
