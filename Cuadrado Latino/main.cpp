#include <windows.h>
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;
vector<int> matriz_latina;
int N = 4;

CRITICAL_SECTION cs;

#define ID_BOTON_GENERAR 1
#define ID_BOTON_MEZCLAR 2
#define ID_CAJA_DIMENSION 3

HWND hCajaDimension;
HWND hVentanaPrincipal;
bool generando = false;

COLORREF ObtenerColorPorValor(int valor) {
    if (valor == 0) return RGB(245, 246, 250);
    int r = (valor * 37) % 180 + 50;
    int g = (valor * 73) % 180 + 50;
    int b = (valor * 111) % 180 + 50;
    return RGB(r, g, b);
}
void GenerarCuadradoLatino() {
    matriz_latina.resize(N * N);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matriz_latina[i * N + j] = ((i + j) % N) + 1;
        }
    }
}

DWORD WINAPI HiloGenerar(LPVOID lpParam) {
    generando = true;
    EnterCriticalSection(&cs);
    
    GenerarCuadradoLatino();
    
    LeaveCriticalSection(&cs);
    generando = false;
    
    InvalidateRect(hVentanaPrincipal, NULL, FALSE);
    return 0;
}

DWORD WINAPI HiloMezclar(LPVOID lpParam) {
    generando = true;
    EnterCriticalSection(&cs);
    for (int k = 0; k < N * 2; k++) {
        int r1 = rand() % N;
        int r2 = rand() % N;
        if (r1 != r2) {
            for (int j = 0; j < N; j++) {
                swap(matriz_latina[r1 * N + j], matriz_latina[r2 * N + j]);
            }
        }
    }
    
    LeaveCriticalSection(&cs);
    generando = false;
    
    InvalidateRect(hVentanaPrincipal, NULL, FALSE);
    return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_CREATE:
            CreateWindowW(L"STATIC", L"Dimensión (N):", WS_CHILD | WS_VISIBLE, 25, 23, 100, 20, hwnd, NULL, NULL, NULL);
            hCajaDimension = CreateWindowW(L"EDIT", L"4", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, 130, 20, 50, 25, hwnd, (HMENU)ID_CAJA_DIMENSION, NULL, NULL);
            
            CreateWindowW(L"BUTTON", L"Generate 🎲", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 200, 18, 110, 28, hwnd, (HMENU)ID_BOTON_GENERAR, NULL, NULL);
            CreateWindowW(L"BUTTON", L"Shuffle 🔀", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 320, 18, 110, 28, hwnd, (HMENU)ID_BOTON_MEZCLAR, NULL, NULL);
            
            GenerarCuadradoLatino();
            break;
            
        case WM_COMMAND:
            if (generando) break;
            
            if (LOWORD(wParam) == ID_BOTON_GENERAR) {
                wchar_t buffer[8];
                GetWindowTextW(hCajaDimension, buffer, 8);
                if (wcslen(buffer) > 0) {
                    int nuevo_n = stoi(wstring(buffer));
                    if (nuevo_n >= 1 && nuevo_n <= 15) {
                        N = nuevo_n;
                        CreateThread(NULL, 0, HiloGenerar, NULL, 0, NULL);
                    } else {
                        MessageBoxW(hwnd, L"Por favor ingresa una dimensión entre 1 y 15.", L"Dimensión inválida", MB_OK | MB_ICONWARNING);
                    }
                }
            }
            else if (LOWORD(wParam) == ID_BOTON_MEZCLAR) {
                CreateThread(NULL, 0, HiloMezclar, NULL, 0, NULL);
            }
            break;
            
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            RECT rect; GetClientRect(hwnd, &rect);
            HDC hdcMem = CreateCompatibleDC(hdc);
            HBITMAP hbmMem = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
            HGDIOBJ hOldObj = SelectObject(hdcMem, hbmMem);
            
            HBRUSH fondo = CreateSolidBrush(RGB(255, 255, 255));
            FillRect(hdcMem, &rect, fondo); DeleteObject(fondo);
            
            EnterCriticalSection(&cs);
            int tam_casilla = (N > 8) ? 35 : 50; 
            int ox = (rect.right - (N * tam_casilla)) / 2;
            int oy = 80 + (rect.bottom - 80 - (N * tam_casilla)) / 2;
            if (oy < 70) oy = 70;
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N; j++) {
                    int valor = matriz_latina[i * N + j];
                    
                    HBRUSH hBrush = CreateSolidBrush(ObtenerColorPorValor(valor));
                    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(44, 62, 80));
                    
                    SelectObject(hdcMem, hBrush);
                    SelectObject(hdcMem, hPen);
                    
                    int x1 = ox + j * tam_casilla;
                    int y1 = oy + i * tam_casilla;
                    int x2 = x1 + tam_casilla;
                    int y2 = y1 + tam_casilla;
                    
                    RoundRect(hdcMem, x1, y1, x2, y2, 6, 6);
                    
                    if (valor > 0) {
                        SetBkMode(hdcMem, TRANSPARENT);
                        SetTextColor(hdcMem, RGB(255, 255, 255));
                        int tam_fuente = (N > 8) ? 16 : 22;
                        HFONT hFont = CreateFontW(tam_fuente, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
                        HGDIOBJ hOldFont = SelectObject(hdcMem, hFont);
                        
                        wstringstream wss; wss << valor; wstring txt = wss.str();
                        RECT rectTexto = {x1, y1, x2, y2};
                        DrawTextW(hdcMem, txt.c_str(), -1, &rectTexto, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                        
                        SelectObject(hdcMem, hOldFont);
                        DeleteObject(hFont);
                    }
                    
                    DeleteObject(hBrush);
                    DeleteObject(hPen);
                }
            }
            
            LeaveCriticalSection(&cs);
            
            BitBlt(hdc, 0, 0, rect.right, rect.bottom, hdcMem, 0, 0, SRCCOPY);
            SelectObject(hdcMem, hOldObj); DeleteObject(hbmMem); DeleteDC(hdcMem);
            
            EndPaint(hwnd, &ps);
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProcW(hwnd, message, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {
    srand((unsigned int)time(NULL));
    InitializeCriticalSection(&cs);
    
    WNDCLASSW wc = {0};
    wc.hbrBackground = (HBRUSH)NULL; 
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = L"MiClaseLatinoDinamico";
    wc.lpfnWndProc = WindowProcedure;
    
    if (!RegisterClassW(&wc)) return -1;
    hVentanaPrincipal = CreateWindowW(L"MiClaseLatinoDinamico", L"Cuadrado Latino Dinámico N x N - Vectores Paralelos", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 680, 560, NULL, NULL, hInst, NULL);
    
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    DeleteCriticalSection(&cs);
    return 0;
}