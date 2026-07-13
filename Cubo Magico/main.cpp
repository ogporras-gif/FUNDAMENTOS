#include <windows.h>
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <map>

using namespace std;

vector<char> cubo(54);

CRITICAL_SECTION cs;

#define ID_BOTON_MEZCLAR 1
#define ID_BOTON_GIRO_U  2
#define ID_BOTON_GIRO_F  3
#define ID_BOTON_GIRO_R  4
#define ID_BOTON_SOLVE   5

HWND hVentanaPrincipal;
bool resolviendo = false;

vector<int> historial_movimientos;

void InicializarCubo() {
    char letras_caras[] = {'W', 'Y', 'G', 'B', 'O', 'R'};
    for (int cara = 0; cara < 6; cara++) {
        for (int i = 0; i < 9; i++) {
            cubo[cara * 9 + i] = letras_caras[cara];
        }
    }
    historial_movimientos.clear();
}

void RotarCaraHorario(int cara) {
    int inicio = cara * 9;
    vector<char> temp(9);
    for(int i = 0; i < 9; i++) temp[i] = cubo[inicio + i];
    
    cubo[inicio + 0] = temp[6]; cubo[inicio + 1] = temp[3]; cubo[inicio + 2] = temp[0];
    cubo[inicio + 3] = temp[7]; cubo[inicio + 4] = temp[4]; cubo[inicio + 5] = temp[1];
    cubo[inicio + 6] = temp[8]; cubo[inicio + 7] = temp[5]; cubo[inicio + 8] = temp[2];
}
void Giro_U(bool registrar = true) {
    RotarCaraHorario(0);
    char t0 = cubo[18], t1 = cubo[19], t2 = cubo[20];
    cubo[18] = cubo[45]; cubo[19] = cubo[46]; cubo[20] = cubo[47];
    cubo[45] = cubo[27]; cubo[46] = cubo[28]; cubo[47] = cubo[29];
    cubo[27] = cubo[36]; cubo[28] = cubo[37]; cubo[29] = cubo[38];
    cubo[36] = t0; cubo[37] = t1; cubo[38] = t2;
    if (registrar && !resolviendo) historial_movimientos.push_back(ID_BOTON_GIRO_U);
}

void Giro_F(bool registrar = true) {
    RotarCaraHorario(2);
    char t0 = cubo[6], t1 = cubo[7], t2 = cubo[8];
    cubo[6] = cubo[44]; cubo[7] = cubo[41]; cubo[8] = cubo[38];
    cubo[44] = cubo[9]; cubo[41] = cubo[10]; cubo[38] = cubo[11];
    cubo[9] = cubo[45]; cubo[10] = cubo[48]; cubo[11] = cubo[51];
    cubo[45] = t0; cubo[48] = t1; cubo[51] = t2;
    if (registrar && !resolviendo) historial_movimientos.push_back(ID_BOTON_GIRO_F);
}

void Giro_R(bool registrar = true) {
    RotarCaraHorario(5);
    char t2 = cubo[2], t5 = cubo[5], t8 = cubo[8];
    cubo[2] = cubo[20]; cubo[5] = cubo[23]; cubo[8] = cubo[26];
    cubo[20] = cubo[11]; cubo[23] = cubo[14]; cubo[26] = cubo[17];
    cubo[11] = cubo[33]; cubo[14] = cubo[30]; cubo[17] = cubo[27];
    cubo[33] = t2; cubo[30] = t5; cubo[27] = t8;
    if (registrar && !resolviendo) historial_movimientos.push_back(ID_BOTON_GIRO_R);
}
DWORD WINAPI HiloMezclar(LPVOID lpParam) {
    EnterCriticalSection(&cs);
    InicializarCubo();
    for (int i = 0; i < 10; i++) {
        int mov = rand() % 3;
        if (mov == 0) Giro_U();
        else if (mov == 1) Giro_F();
        else Giro_R();
    }
    LeaveCriticalSection(&cs);
    InvalidateRect(hVentanaPrincipal, NULL, FALSE);
    return 0;
}

DWORD WINAPI HiloGiro(LPVOID lpParam) {
    int tipo = (int)(intptr_t)lpParam;
    EnterCriticalSection(&cs);
    if (tipo == ID_BOTON_GIRO_U) Giro_U();
    else if (tipo == ID_BOTON_GIRO_F) Giro_F();
    else if (tipo == ID_BOTON_GIRO_R) Giro_R();
    LeaveCriticalSection(&cs);
    InvalidateRect(hVentanaPrincipal, NULL, FALSE);
    return 0;
}

DWORD WINAPI HiloResolver(LPVOID lpParam) {
    resolviendo = true;
    for (int i = (int)historial_movimientos.size() - 1; i >= 0; i--) {
        int mov_inverso = historial_movimientos[i];
        
        EnterCriticalSection(&cs);
        if (mov_inverso == ID_BOTON_GIRO_U) { Giro_U(false); Giro_U(false); Giro_U(false); }
        else if (mov_inverso == ID_BOTON_GIRO_F) { Giro_F(false); Giro_F(false); Giro_F(false); }
        else if (mov_inverso == ID_BOTON_GIRO_R) { Giro_R(false); Giro_R(false); Giro_R(false); }
        LeaveCriticalSection(&cs);
        
        InvalidateRect(hVentanaPrincipal, NULL, FALSE);
        Sleep(400);
    }
    
    historial_movimientos.clear();
    resolviendo = false;
    return 0;
}

// --- DIBUJADO DE CARAS ---
COLORREF ObtenerColorPegatina(char c) {
    switch(c) {
        case 'W': return RGB(245, 246, 250);
        case 'Y': return RGB(241, 196, 15);
        case 'G': return RGB(46, 204, 113);
        case 'B': return RGB(41, 128, 185);
        case 'O': return RGB(230, 126, 34);
        case 'R': return RGB(231, 76, 60);
        default:  return RGB(52, 73, 94);
    }
}

void DibujarCara(HDC hdc, int cara, int inicio_x, int inicio_y, int tam_celda) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int indice_pegatina = cara * 9 + (i * 3 + j);
            char color_char = cubo[indice_pegatina];
            
            HBRUSH hBrush = CreateSolidBrush(ObtenerColorPegatina(color_char));
            HPEN hPen = CreatePen(PS_SOLID, 2, RGB(30, 39, 46));
            
            SelectObject(hdc, hBrush);
            SelectObject(hdc, hPen);
            
            int x1 = inicio_x + j * tam_celda;
            int y1 = inicio_y + i * tam_celda;
            int x2 = x1 + tam_celda;
            int y2 = y1 + tam_celda;
            
            RoundRect(hdc, x1, y1, x2, y2, 6, 6);
            
            DeleteObject(hBrush);
            DeleteObject(hPen);
        }
    }
}

// --- INTERFAZ WINDOWS ---
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_CREATE:
            CreateWindowW(L"BUTTON", L"Scramble 🎲", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 25, 20, 110, 35, hwnd, (HMENU)ID_BOTON_MEZCLAR, NULL, NULL);
            CreateWindowW(L"BUTTON", L"Giro U ⬆️", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 145, 20, 100, 35, hwnd, (HMENU)ID_BOTON_GIRO_U, NULL, NULL);
            CreateWindowW(L"BUTTON", L"Giro F ➡️", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 255, 20, 100, 35, hwnd, (HMENU)ID_BOTON_GIRO_F, NULL, NULL);
            CreateWindowW(L"BUTTON", L"Giro R ↗️", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 365, 20, 100, 35, hwnd, (HMENU)ID_BOTON_GIRO_R, NULL, NULL);
            CreateWindowW(L"BUTTON", L"Solve ✨", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 475, 20, 110, 35, hwnd, (HMENU)ID_BOTON_SOLVE, NULL, NULL);
            InicializarCubo();
            break;
            
        case WM_COMMAND:
            if (resolviendo) break;
            
            if (LOWORD(wParam) == ID_BOTON_MEZCLAR) {
                CreateThread(NULL, 0, HiloMezclar, NULL, 0, NULL);
            }
            else if (LOWORD(wParam) >= ID_BOTON_GIRO_U && LOWORD(wParam) <= ID_BOTON_GIRO_R) {
                CreateThread(NULL, 0, HiloGiro, (LPVOID)(intptr_t)LOWORD(wParam), 0, NULL);
            }
            else if (LOWORD(wParam) == ID_BOTON_SOLVE) {
                if(!historial_movimientos.empty()) {
                    CreateThread(NULL, 0, HiloResolver, NULL, 0, NULL);
                }
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
            int tam = 38;
            int ox = 50, oy = 90;
            
            DibujarCara(hdcMem, 0, ox + 3 * tam, oy, tam);           // U
            DibujarCara(hdcMem, 4, ox, oy + 3 * tam, tam);           // L
            DibujarCara(hdcMem, 2, ox + 3 * tam, oy + 3 * tam, tam); // F
            DibujarCara(hdcMem, 5, ox + 6 * tam, oy + 3 * tam, tam); // R
            DibujarCara(hdcMem, 3, ox + 9 * tam, oy + 3 * tam, tam); // B
            DibujarCara(hdcMem, 1, ox + 3 * tam, oy + 6 * tam, tam); // D
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
    wc.lpszClassName = L"MiClaseRubikSolver";
    wc.lpfnWndProc = WindowProcedure;
    
    if (!RegisterClassW(&wc)) return -1;
    
    hVentanaPrincipal = CreateWindowW(L"MiClaseRubikSolver", L"Cubo de Rubik - Solucionador Animado por Historial", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 620, 500, NULL, NULL, hInst, NULL);
    
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    DeleteCriticalSection(&cs);
    return 0;
}