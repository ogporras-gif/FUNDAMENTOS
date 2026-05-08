#include "Persona.h"
#include <cctype>

Persona::Persona() {
}

Persona::Persona(string n, string c) {
    nombre = n;
    cedula = c;
}

string Persona::getNombre() {
    return nombre;
}

string Persona::getCedula() {
    return cedula;
}

bool Persona::validarCedula() {

    if (cedula.length() != 10)
        return false;

    for (int i = 0; i < 10; i++) {

        if (!isdigit(cedula.at(i)))
            return false;
    }

    int provincia =
        (cedula.at(0) - '0') * 10 +
        (cedula.at(1) - '0');

    if (provincia < 1 || provincia > 24)
        return false;

    int tercer = cedula.at(2) - '0';

    if (tercer >= 6)
        return false;

    int suma = 0;

    for (int i = 0; i < 9; i++) {

        int dig = cedula.at(i) - '0';

        if (i % 2 == 0) {

            dig *= 2;

            if (dig > 9)
                dig -= 9;
        }

        suma += dig;
    }

    int verificador =
        (10 - (suma % 10)) % 10;

    return verificador ==
           (cedula.at(9) - '0');
}

string Persona::obtenerProvincia() {

    int cod =
        (cedula.at(0) - '0') * 10 +
        (cedula.at(1) - '0');

    switch(cod) {

        case 1: return "Azuay";
        case 2: return "Bolivar";
        case 3: return "Canar";
        case 4: return "Carchi";
        case 5: return "Cotopaxi";
        case 6: return "Chimborazo";
        case 7: return "El Oro";
        case 8: return "Esmeraldas";
        case 9: return "Guayas";
        case 10: return "Imbabura";
        case 11: return "Loja";
        case 12: return "Los Rios";
        case 13: return "Manabi";
        case 14: return "Morona Santiago";
        case 15: return "Napo";
        case 16: return "Pastaza";
        case 17: return "Pichincha";
        case 18: return "Tungurahua";
        case 19: return "Zamora Chinchipe";
        case 20: return "Galapagos";
        case 21: return "Sucumbios";
        case 22: return "Orellana";
        case 23: return "Santo Domingo";
        case 24: return "Santa Elena";

        default:
            return "Desconocida";
    }
}

void Persona::mostrar() {

    cout << "Nombre: " << nombre
         << " | Cedula: " << cedula
         << " | Provincia: "
         << obtenerProvincia()
         << endl;
}
