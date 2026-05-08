#include "Persona.h"

Persona::Persona() {
    nombre = "";
    cedula = "";
}

Persona::Persona(string nombre, string cedula) {
    this->nombre = nombre;
    this->cedula = cedula;
}

string Persona::getNombre() {
    return nombre;
}

string Persona::getCedula() {
    return cedula;
}

void Persona::setNombre(string nombre) {
    this->nombre = nombre;
}

void Persona::setCedula(string cedula) {
    this->cedula = cedula;
}
