#include "NReinas.h"

NReinas::NReinas(int tam)
{
    n=tam;
    soluciones=0;

    tablero=new int*[n];

    for(int i=0;i<n;i++)
    {
        tablero[i]=new int[n];

        for(int j=0;j<n;j++)
            tablero[i][j]=0;
    }

    archivo.open("soluciones.txt");
}

NReinas::~NReinas()
{
    for(int i=0;i<n;i++)
        delete[] tablero[i];

    delete[] tablero;

    archivo.close();
}

bool NReinas::esSeguro(int fila,int columna)
{

    for(int i=0;i<columna;i++)
    {
        if(tablero[fila][i]==1)
            return false;
    }

    for(int i=fila,j=columna;i>=0 && j>=0;i--,j--)
    {
        if(tablero[i][j]==1)
            return false;
    }

    for(int i=fila,j=columna;i<n && j>=0;i++,j--)
    {
        if(tablero[i][j]==1)
            return false;
    }

    return true;
}

void NReinas::imprimir()
{
    soluciones++;

    cout<<"\nSOLUCION "<<soluciones<<"\n\n";

    archivo<<"SOLUCION "<<soluciones<<"\n\n";

    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            if(tablero[i][j]==1)
            {
                cout<<" Q ";
                archivo<<" Q ";
            }
            else
            {
                cout<<" . ";
                archivo<<" . ";
            }
        }

        cout<<endl;
        archivo<<endl;
    }

    cout<<endl;

    archivo<<endl;
}

void NReinas::resolver(int columna)
{
    if(columna==n)
    {
        imprimir();
        return;
    }

    for(int fila=0;fila<n;fila++)
    {
        if(esSeguro(fila,columna))
        {
            tablero[fila][columna]=1;

            resolver(columna+1);

            tablero[fila][columna]=0;
        }
    }
}

void NReinas::iniciar()
{
    resolver(0);

    cout<<"\nTOTAL DE SOLUCIONES: "<<soluciones<<endl;

    archivo<<"\nTOTAL DE SOLUCIONES: "<<soluciones<<endl;
}
