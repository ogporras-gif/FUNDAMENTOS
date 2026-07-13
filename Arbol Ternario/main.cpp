#include <windows.h>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

vector<int> valores;
vector<int> izq;
vector<int> centro; 
vector<int> der;
int raiz = -1;

CRITICAL_SECTION cs;

#define ID_BOTON_INSERTAR 1
#define ID_CAJA_NUMERO    2
#define ID_BOTON_BORRAR   3

HWND hCajaNumero;
HWND hVentanaPrincipal;

int InsertarTernario(int nodo_idx, int valor, int nuevo_idx) {
    if (nodo_idx == -1) return nuevo_idx;

    if (valor < valores[nodo_idx]) {
        izq[nodo_idx] = InsertarTernario(izq[nodo_idx], valor, nuevo_idx);
    } else if (valor == valores[nodo_idx]) {
        centro[nodo_idx] = InsertarTernario(centro[nodo_idx], valor, nuevo_idx);
    } else {
        der[nodo_idx] = InsertarTernario(der[nodo_idx], valor, nuevo_idx);
    }
    return nodo_idx;
}
int EncontrarMinimo(int nodo_idx) {
    while (nodo_idx != -1 && izq[nodo_idx] != -1) {
        nodo_idx = izq[nodo_idx];
    }
    return nodo_idx;
}

int BorrarTernario(int nodo_idx, int valor) {
    if (nodo_idx == -1 || nodo_idx >= (int)valores.size()) return -1;

    if (valor < valores[nodo_idx]) {
        izq[nodo_idx] = BorrarTernario(izq[nodo_idx], valor);
    } else if (valor > valores[nodo_idx]) {
        der[nodo_idx] = BorrarTernario(der[nodo_idx], valor);
    } else {
        if (centro[nodo_idx] != -1) {
            centro[nodo_idx] = BorrarTernario(centro[nodo_idx], valor);
        } 
        else if (izq[nodo_idx] == -1) {
            return der[nodo_idx];
        } else if (der[nodo_idx] == -1) {
            return izq[nodo_idx];
        } 
        else {
            int min_idx = EncontrarMinimo(der[nodo_idx]);
            valores[nodo_idx] = valores[min_idx];
            centro[nodo_idx] = centro[min_idx]; 
            centro[min_idx] = -1; 
            der[nodo_idx] = BorrarTernario(der[nodo_idx], valores[min_idx]);
        }
    }
    return nodo_idx;
}
DWORD WINAPI HiloInsercion(LPVOID lpParam) {
    int val = (int)(intptr_t)lpParam;
    EnterCriticalSection(&cs);
    valores.push_back(val); izq.push_back(-1); centro.push_back(-1); der.push_back(-1);
    if (raiz == -1) raiz = (int)valores.size() - 1;
    else InsertarTernario(raiz, val, (int)valores.size() - 1);
    LeaveCriticalSection(&cs);
    InvalidateRect(hVentanaPrincipal, NULL, FALSE);
    return 0;
}

DWORD WINAPI HiloBorrado(LPVOID lpParam) {
    int val = (int)(intptr_t)lpParam;
    EnterCriticalSection(&cs);
    raiz = BorrarTernario(raiz, val);
    LeaveCriticalSection(&cs);
    InvalidateRect(hVentanaPrincipal, NULL, FALSE);
    return 0;
}
void DibujarArbol(HDC hdc, int nodo_idx, float x, float y, float desp_x) {
    if (nodo_idx == -1 || nodo_idx >= (int)valores.size()) return;
    int radio = 22;

    if (izq[nodo_idx] != -1) {
        HPEN hPen = CreatePen(PS_SOLID, 2, RGB(180, 180, 180)); SelectObject(hdc, hPen);
        MoveToEx(hdc, (int)x, (int)y, NULL); LineTo(hdc, (int)(x - desp_x), (int)(y + 80));
        DeleteObject(hPen); DibujarArbol(hdc, izq[nodo_idx], x - desp_x, y + 80, desp_x * 0.4f);
    }
    if (centro[nodo_idx] != -1) {
        HPEN hPen = CreatePen(PS_DASH, 2, RGB(230, 126, 34)); SelectObject(hdc, hPen);
        MoveToEx(hdc, (int)x, (int)y, NULL); LineTo(hdc, (int)x, (int)(y + 80));
        DeleteObject(hPen); DibujarArbol(hdc, centro[nodo_idx], x, y + 80, desp_x * 0.4f);
    }
    if (der[nodo_idx] != -1) {
        HPEN hPen = CreatePen(PS_SOLID, 2, RGB(180, 180, 180)); SelectObject(hdc, hPen);
        MoveToEx(hdc, (int)x, (int)y, NULL); LineTo(hdc, (int)(x + desp_x), (int)(y + 80));
        DeleteObject(hPen); DibujarArbol(hdc, der[nodo_idx], x + desp_x, y + 80, desp_x * 0.4f);
    }

    HBRUSH hBrush = CreateSolidBrush(RGB(26, 188, 156)); HPEN hPenNodo = CreatePen(PS_SOLID, 1, RGB(26, 188, 156));
    SelectObject(hdc, hBrush); SelectObject(hdc, hPenNodo);
    Ellipse(hdc, (int)(x - radio), (int)(y - radio), (int)(x + radio), (int)(y + radio));
    DeleteObject(hBrush); DeleteObject(hPenNodo);

    SetBkMode(hdc, TRANSPARENT); SetTextColor(hdc, RGB(255, 255, 255));
    wstringstream wss; wss << valores[nodo_idx]; wstring txt = wss.str();
    RECT rectTexto = {(int)(x - radio), (int)(y - radio), (int)(x + radio), (int)(y + radio)};
    DrawTextW(hdc, txt.c_str(), -1, &rectTexto, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_CREATE:
            CreateWindowW(L"STATIC", L"Número:", WS_CHILD | WS_VISIBLE, 20, 18, 65, 20, hwnd, NULL, NULL, NULL);
            hCajaNumero = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, 90, 15, 80, 25, hwnd, (HMENU)ID_CAJA_NUMERO, NULL, NULL);
            CreateWindowW(L"BUTTON", L"Insertar", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 185, 13, 100, 28, hwnd, (HMENU)ID_BOTON_INSERTAR, NULL, NULL);
            CreateWindowW(L"BUTTON", L"Borrar Número", WS_CHILD | WS_VISIBLE, 300, 13, 120, 28, hwnd, (HMENU)ID_BOTON_BORRAR, NULL, NULL);
            break;
        case WM_COMMAND:
            if (LOWORD(wParam) == ID_BOTON_INSERTAR || LOWORD(wParam) == ID_BOTON_BORRAR) {
                wchar_t buffer[16]; GetWindowTextW(hCajaNumero, buffer, 16);
                if (wcslen(buffer) > 0) {
                    int val = stoi(wstring(buffer)); SetWindowTextW(hCajaNumero, L"");
                    if (LOWORD(wParam) == ID_BOTON_INSERTAR) CreateThread(NULL, 0, HiloInsercion, (LPVOID)(intptr_t)val, 0, NULL);
                    else CreateThread(NULL, 0, HiloBorrado, (LPVOID)(intptr_t)val, 0, NULL);
                }
            }
            break;
        case WM_PAINT: {
            PAINTSTRUCT ps; HDC hdc = BeginPaint(hwnd, &ps);
            HBRUSH fondo = CreateSolidBrush(RGB(255, 255, 255)); FillRect(hdc, &ps.rcPaint, fondo); DeleteObject(fondo);
            EnterCriticalSection(&cs);
            if (raiz != -1) { RECT rect; GetClientRect(hwnd, &rect); DibujarArbol(hdc, raiz, rect.right / 2.0f, 130.0f, rect.right / 3.5f); }
            LeaveCriticalSection(&cs);
            EndPaint(hwnd, &ps);
            break;
        }
        case WM_DESTROY: DeleteCriticalSection(&cs); PostQuitMessage(0); break;
        default: return DefWindowProcW(hwnd, message, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {
    InitializeCriticalSection(&cs);
    WNDCLASSW wc = {0}; wc.hbrBackground = (HBRUSH)NULL; wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInst; wc.lpszClassName = L"MiClaseTerDef"; wc.lpfnWndProc = WindowProcedure;
    if (!RegisterClassW(&wc)) return -1;
    hVentanaPrincipal = CreateWindowW(L"MiClaseTerDef", L"Árbol Ternario Dinámico - Fondo Blanco con Borrado", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 1024, 720, NULL, NULL, hInst, NULL);
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) { TranslateMessage(&msg); DispatchMessage(&msg); }
    return 0;
}