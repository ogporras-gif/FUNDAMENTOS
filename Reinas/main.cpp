#include "NReinas.h"

int main()
{

    int n;

    cout<<"Ingrese el numero de reinas: ";
    cin>>n;

    if(n<=0)
    {
        cout<<"Numero incorrecto";
        return 0;
    }

    NReinas juego(n);

    juego.iniciar();

    return 0;
}
