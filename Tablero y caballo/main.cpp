#include <windows.h>
#include <iostream>
#include <fstream>

using namespace std;

//================ VARIABLES GLOBALES =================
int N;
int sizeCell;

int **tablero = NULL; 

int filaCaballo;
int colCaballo;

HWND g_hwnd;

//================ ACTIVAR CONSOLA =================
void activarConsola()
{
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONIN$", "r", stdin);
}

//================ DIBUJAR RECTÁNGULO =================
void Color(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color)
{
    HBRUSH brush = CreateSolidBrush(color);
    RECT r = {x1, y1, x2, y2};
    FillRect(hdc, &r, brush);
    DeleteObject(brush);
}

//================ GENERAR ARCHIVO =================
void GenerarArchivo()
{
    ofstream archivo("tablero.txt");
    archivo << N << endl;

    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            int valor;

            if((i + j) % 2 == 0)
            {
                valor = 1; 
            }
            else
            {
                valor = 0; 
            }

            archivo << valor << " ";
        }
        archivo << endl;
    }
    archivo.close();
}

//================ LEER ARCHIVO (CON PUNTEROS CORREGIDOS) =================
void LeerArchivo()
{
    ifstream archivo("tablero.txt");
    if (!archivo.is_open()) return;

    archivo >> N;
    archivo.ignore(); 
    tablero = new int*[N];
    for(int i = 0; i < N; i++)
    {
        *(tablero + i) = new int[N];
    }
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            int temp;
            archivo >> temp;
            *(*(tablero + i) + j) = temp;
        }
    }
    archivo.close();
}

//========= CALCULAR MOVIMIENTOS (CON PUNTEROS CORREGIDOS) =========
void CalcularMovimientosCaballo()
{
    *(*(tablero + filaCaballo) + colCaballo) = 2;
    int desplFilas[] = {-2, -2, -1, -1,  1, 1,  2, 2};
    int desplCols[]  = {-1,  1, -2,  2, -2, 2, -1, 1};

    for (int k = 0; k < 8; k++)
    {
        int nuevaFila = filaCaballo + *(desplFilas + k);
        int nuevaCol  = colCaballo + *(desplCols + k);

        if (nuevaFila >= 0 && nuevaFila < N && nuevaCol >= 0 && nuevaCol < N)
        {
            *(*(tablero + nuevaFila) + nuevaCol) = 3;
        }
    }
}

//================ DIBUJAR TABLERO (CON PUNTEROS PUROS) =================
void Dibujar(HDC hdc)
{
    if (tablero == NULL) return;

    int anchoTablero = N * sizeCell;
    int altoTablero  = N * sizeCell;

    int anchoPantalla = GetSystemMetrics(SM_CXSCREEN);
    int altoPantalla  = GetSystemMetrics(SM_CYSCREEN);

    int offsetX = (anchoPantalla - anchoTablero) / 2;
    int offsetY = (altoPantalla - altoTablero) / 2;
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            int x = offsetX + j * sizeCell;
            int y = offsetY + i * sizeCell;

            int valorCasilla = *(*(tablero + i) + j);

            if (valorCasilla == 3)
            {
                Color(hdc, x, y, x + sizeCell, y + sizeCell, RGB(46, 204, 113));
            }
            else if(valorCasilla == 1 || (valorCasilla == 2 && (i + j) % 2 == 0))
            {
                Color(hdc, x, y, x + sizeCell, y + sizeCell, RGB(255, 255, 255));
            }
            else
            {
                Color(hdc, x, y, x + sizeCell, y + sizeCell, RGB(40, 40, 40));
            }
        }
    }
    HPEN pen = CreatePen(PS_SOLID, 1, RGB(150, 150, 150));
    SelectObject(hdc, pen);

    for(int i = 0; i <= N; i++)
    {
        MoveToEx(hdc, offsetX, offsetY + i * sizeCell, NULL);
        LineTo(hdc, offsetX + anchoTablero, offsetY + i * sizeCell);
    }
    for(int j = 0; j <= N; j++)
    {
        MoveToEx(hdc, offsetX + j * sizeCell, offsetY, NULL);
        LineTo(hdc, offsetX + j * sizeCell, offsetY + altoTablero);
    }
    DeleteObject(pen);
    HFONT hFont = CreateFontW(sizeCell * 0.75, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, 
                              DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
                              DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI Symbol");
    HFONT oldFont = (HFONT)SelectObject(hdc, hFont);
    SetBkMode(hdc, TRANSPARENT); 

    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            if(*(*(tablero + i) + j) == 2) 
            {
                int x = offsetX + j * sizeCell;
                int y = offsetY + i * sizeCell;

                if((i + j) % 2 == 0)
                    SetTextColor(hdc, RGB(20, 20, 20));    
                else
                    SetTextColor(hdc, RGB(255, 215, 0));   

                wchar_t caballo[] = { 0x265E, 0 }; 

                int paddingX = sizeCell * 0.15;
                int paddingY = sizeCell * 0.08;

                TextOutW(hdc, x + paddingX, y + paddingY, caballo, 1);
            }
        }
    }

    SelectObject(hdc, oldFont);
    DeleteObject(hFont);
}

//================ PROCEDIMIENTO DE LA VENTANA =================
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch(msg)
    {
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            Dibujar(hdc);
            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_DESTROY:
        {
            if(tablero != NULL)
            {
                for(int i = 0; i < N; i++)
                {
                    delete[] *(tablero + i);
                }
                delete[] tablero;
                tablero = NULL;
            }
            PostQuitMessage(0);
            return 0;
        }
    }
    return DefWindowProc(hwnd, msg, wp, lp);
}

//================ FUNCIÓN PRINCIPAL =================
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE,
                   LPSTR,
                   int)
{
    activarConsola();
    
    do
    {
        cout << "Ingrese el tamaño del tablero (1 - 8): ";
        cin >> N;

        if(N < 1 || N > 8)
        {
            cout << "Error. Debe ingresar un numero entre 1 y 8.\n\n";
        }
    } while(N < 1 || N > 8);

    GenerarArchivo();
    LeerArchivo();

    do
    {
        cout << "Ingrese la FILA donde estara el caballo (0 a " << N - 1 << "): ";
        cin >> filaCaballo;
        cout << "Ingrese la COLUMNA donde estara el caballo (0 a " << N - 1 << "): ";
        cin >> colCaballo;

        if (filaCaballo < 0 || filaCaballo >= N || colCaballo < 0 || colCaballo >= N)
        {
            cout << "Error: Posicion fuera de limites. Intente de nuevo.\n\n";
        }
    } while(filaCaballo < 0 || filaCaballo >= N || colCaballo < 0 || colCaballo >= N);

    CalcularMovimientosCaballo();

    int anchoPantalla = GetSystemMetrics(SM_CXSCREEN);
    int altoPantalla  = GetSystemMetrics(SM_CYSCREEN);

    sizeCell = min((anchoPantalla - 100) / N,
                   (altoPantalla - 100) / N);

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = TEXT("Ajedrez"); 

    RegisterClass(&wc);

    HWND hwnd = CreateWindow(
        TEXT("Ajedrez"),            
        TEXT("Tablero de Ajedrez Dinamico"),  
        WS_OVERLAPPEDWINDOW,
        0,
        0,
        anchoPantalla,
        altoPantalla,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    g_hwnd = hwnd;

    ShowWindow(hwnd, SW_MAXIMIZE);
    UpdateWindow(hwnd);

    InvalidateRect(hwnd, NULL, TRUE);
    UpdateWindow(hwnd);

    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}