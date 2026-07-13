#include <windows.h>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

#define NEGRO 0
#define ROJO  1

vector<int> valores;
vector<int> izq;
vector<int> der;
vector<int> padres;
vector<int> colores;
int raiz = -1;

CRITICAL_SECTION cs;

#define ID_BOTON_INSERTAR 1
#define ID_CAJA_NUMERO    2
#define ID_BOTON_BORRAR   3

HWND hCajaNumero;
HWND hVentanaPrincipal;

void RotarIzquierda(int& root, int x) {
    int y = der[x];
    der[x] = izq[y];
    if (izq[y] != -1) padres[izq[y]] = x;
    padres[y] = padres[x];
    if (padres[x] == -1) root = y;
    else if (x == izq[padres[x]]) izq[padres[x]] = y;
    else der[padres[x]] = y;
    izq[y] = x;
    padres[x] = y;
}

void RotarDerecha(int& root, int y) {
    int x = izq[y];
    izq[y] = der[x];
    if (der[x] != -1) padres[der[x]] = y;
    padres[x] = padres[y];
    if (padres[y] == -1) root = x;
    else if (y == izq[padres[y]]) izq[padres[y]] = x;
    else der[padres[y]] = x;
    der[x] = y;
    padres[y] = x;
}
void CorregirInsercion(int& root, int z) {
    while (z != root && colores[padres[z]] == ROJO) {
        if (padres[z] == izq[padres[padres[z]]]) {
            int y = der[padres[padres[z]]];
            if (y != -1 && colores[y] == ROJO) {
                colores[padres[z]] = NEGRO;
                colores[y] = NEGRO;
                colores[padres[padres[z]]] = ROJO;
                z = padres[padres[z]];
            } else {
                if (z == der[padres[z]]) {
                    z = padres[z];
                    RotarIzquierda(root, z);
                }
                colores[padres[z]] = NEGRO;
                colores[padres[padres[z]]] = ROJO;
                RotarDerecha(root, padres[padres[z]]);
            }
        } else {
            int y = izq[padres[padres[z]]];
            if (y != -1 && colores[y] == ROJO) {
                colores[padres[z]] = NEGRO;
                colores[y] = NEGRO;
                colores[padres[padres[z]]] = ROJO;
                z = padres[padres[z]];
            } else {
                if (z == izq[padres[z]]) {
                    z = padres[z];
                    RotarDerecha(root, z);
                }
                colores[padres[z]] = NEGRO;
                colores[padres[padres[z]]] = ROJO;
                RotarIzquierda(root, padres[padres[z]]);
            }
        }
    }
    colores[root] = NEGRO;
}

void Insertar(int valor) {
    EnterCriticalSection(&cs);
    valores.push_back(valor);
    izq.push_back(-1);
    der.push_back(-1);
    padres.push_back(-1);
    colores.push_back(ROJO);
    int z = (int)valores.size() - 1;

    int y = -1;
    int x = raiz;

    while (x != -1) {
        y = x;
        if (valores[z] < valores[x]) x = izq[x];
        else x = der[x];
    }

    padres[z] = y;
    if (y == -1) raiz = z;
    else if (valores[z] < valores[y]) izq[y] = z;
    else der[y] = z;

    CorregirInsercion(raiz, z);
    LeaveCriticalSection(&cs);
    InvalidateRect(hVentanaPrincipal, NULL, FALSE);
}
int BuscarNodo(int nodo_idx, int k) {
    if (nodo_idx == -1 || valores[nodo_idx] == k) return nodo_idx;
    if (k < valores[nodo_idx]) return BuscarNodo(izq[nodo_idx], k);
    return BuscarNodo(der[nodo_idx], k);
}

void CorregirBorrado(int& root, int x, int x_padre) {
    while (x != root && (x == -1 || colores[x] == NEGRO)) {
        if (x == izq[x_padre]) {
            int w = der[x_padre];
            if (w != -1 && colores[w] == ROJO) {
                colores[w] = NEGRO;
                colores[x_padre] = ROJO;
                RotarIzquierda(root, x_padre);
                w = der[x_padre];
            }
            if ((w == -1 || izq[w] == -1 || colores[izq[w]] == NEGRO) &&
                (w == -1 || der[w] == -1 || colores[der[w]] == NEGRO)) {
                if (w != -1) colores[w] = ROJO;
                x = x_padre;
                x_padre = padres[x];
            } else {
                if (w != -1 && (der[w] == -1 || colores[der[w]] == NEGRO)) {
                    if (izq[w] != -1) colores[izq[w]] = NEGRO;
                    colores[w] = ROJO;
                    RotarDerecha(root, w);
                    w = der[x_padre];
                }
                if (w != -1) {
                    colores[w] = colores[x_padre];
                    if (der[w] != -1) colores[der[w]] = NEGRO;
                }
                colores[x_padre] = NEGRO;
                RotarIzquierda(root, x_padre);
                x = root;
                break;
            }
        } else {
            int w = izq[x_padre];
            if (w != -1 && colores[w] == ROJO) {
                colores[w] = NEGRO;
                colores[x_padre] = ROJO;
                RotarDerecha(root, x_padre);
                w = izq[x_padre];
            }
            if ((w == -1 || der[w] == -1 || colores[der[w]] == NEGRO) &&
                (w == -1 || izq[w] == -1 || colores[izq[w]] == NEGRO)) {
                if (w != -1) colores[w] = ROJO;
                x = x_padre;
                x_padre = padres[x];
            } else {
                if (w != -1 && (izq[w] == -1 || colores[izq[w]] == NEGRO)) {
                    if (der[w] != -1) colores[der[w]] = NEGRO;
                    colores[w] = ROJO;
                    RotarIzquierda(root, w);
                    w = izq[x_padre];
                }
                if (w != -1) {
                    colores[w] = colores[x_padre];
                    if (izq[w] != -1) colores[izq[w]] = NEGRO;
                }
                colores[x_padre] = NEGRO;
                RotarDerecha(root, x_padre);
                x = root;
                break;
            }
        }
    }
    if (x != -1) colores[x] = NEGRO;
}

void BorrarNodoFisico(int& root, int z) {
    int y = z;
    int y_color_original = colores[y];
    int x, x_padre;

    if (izq[z] == -1) {
        x = der[z];
        x_padre = padres[z];
        if (der[z] != -1) padres[der[z]] = padres[z];
        if (padres[z] == -1) root = der[z];
        else if (z == izq[padres[z]]) izq[padres[z]] = der[z];
        else der[padres[z]] = der[z];
    } else if (der[z] == -1) {
        x = izq[z];
        x_padre = padres[z];
        if (izq[z] != -1) padres[izq[z]] = padres[z];
        if (padres[z] == -1) root = izq[z];
        else if (z == izq[padres[z]]) izq[padres[z]] = izq[z];
        else der[padres[z]] = izq[z];
    } else {
        y = der[z];
        while (izq[y] != -1) y = izq[y];
        y_color_original = colores[y];
        x = der[y];
        if (padres[y] == z) x_padre = y;
        else {
            x_padre = padres[y];
            if (der[y] != -1) padres[der[y]] = padres[y];
            izq[padres[y]] = der[y];
            der[y] = der[z];
            padres[der[z]] = y;
        }
        if (padres[z] == -1) root = y;
        else if (z == izq[padres[z]]) izq[padres[z]] = y;
        else der[padres[z]] = y;
        padres[y] = padres[z];
        izq[y] = izq[z];
        padres[izq[z]] = y;
        colores[y] = colores[z];
    }

    if (y_color_original == NEGRO) {
        CorregirBorrado(root, x, x_padre);
    }
}

void Borrar(int valor) {
    EnterCriticalSection(&cs);
    int z = BuscarNodo(raiz, valor);
    if (z != -1) {
        BorrarNodoFisico(raiz, z);
    }
    LeaveCriticalSection(&cs);
    InvalidateRect(hVentanaPrincipal, NULL, FALSE);
}

DWORD WINAPI HiloInsercion(LPVOID lpParam) {
    Insertar((int)(intptr_t)lpParam);
    return 0;
}

DWORD WINAPI HiloBorrado(LPVOID lpParam) {
    Borrar((int)(intptr_t)lpParam);
    return 0;
}

void DibujarArbolRN(HDC hdc, int nodo_idx, float x, float y, float desp_x) {
    if (nodo_idx == -1 || nodo_idx >= (int)valores.size()) return;
    int radio = 22;

    if (izq[nodo_idx] != -1) {
        HPEN hPen = CreatePen(PS_SOLID, 2, RGB(160, 160, 160)); SelectObject(hdc, hPen);
        MoveToEx(hdc, (int)x, (int)y, NULL); LineTo(hdc, (int)(x - desp_x), (int)(y + 80));
        DeleteObject(hPen); DibujarArbolRN(hdc, izq[nodo_idx], x - desp_x, y + 80, desp_x * 0.5f);
    }
    if (der[nodo_idx] != -1) {
        HPEN hPen = CreatePen(PS_SOLID, 2, RGB(160, 160, 160)); SelectObject(hdc, hPen);
        MoveToEx(hdc, (int)x, (int)y, NULL); LineTo(hdc, (int)(x + desp_x), (int)(y + 80));
        DeleteObject(hPen); DibujarArbolRN(hdc, der[nodo_idx], x + desp_x, y + 80, desp_x * 0.5f);
    }

    HBRUSH hBrush;
    if (colores[nodo_idx] == ROJO) hBrush = CreateSolidBrush(RGB(231, 76, 60));
    else hBrush = CreateSolidBrush(RGB(44, 62, 80));

    HPEN hPenNodo = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    SelectObject(hdc, hBrush); SelectObject(hdc, hPenNodo);
    Ellipse(hdc, (int)(x - radio), (int)(y - radio), (int)(x + radio), (int)(y + radio));
    DeleteObject(hBrush); DeleteObject(hPenNodo);

    SetBkMode(hdc, TRANSPARENT); SetTextColor(hdc, RGB(255, 255, 255));
    wstringstream wss; wss << valores[nodo_idx]; wstring txt = wss.str();
    RECT rectTexto = {(int)(x - radio), (int)(y - radio), (int)(x + radio), (int)(y + radio)};
    DrawTextW(hdc, txt.c_str(), -1, &rectTexto, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

// --- INTERFAZ WINDOWS ---
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_CREATE:
            CreateWindowW(L"STATIC", L"Número:", WS_CHILD | WS_VISIBLE, 20, 18, 65, 20, hwnd, NULL, NULL, NULL);
            hCajaNumero = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, 90, 15, 80, 25, hwnd, (HMENU)ID_CAJA_NUMERO, NULL, NULL);
            CreateWindowW(L"BUTTON", L"Insertar RN", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 185, 13, 100, 28, hwnd, (HMENU)ID_BOTON_INSERTAR, NULL, NULL);
            CreateWindowW(L"BUTTON", L"Borrar RN", WS_CHILD | WS_VISIBLE, 300, 13, 100, 28, hwnd, (HMENU)ID_BOTON_BORRAR, NULL, NULL);
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
            if (raiz != -1) { RECT rect; GetClientRect(hwnd, &rect); DibujarArbolRN(hdc, raiz, rect.right / 2.0f, 130.0f, rect.right / 4.0f); }
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
    wc.hInstance = hInst; wc.lpszClassName = L"MiClaseArbolRN"; wc.lpfnWndProc = WindowProcedure;
    if (!RegisterClassW(&wc)) return -1;
    hVentanaPrincipal = CreateWindowW(L"MiClaseArbolRN", L"Árbol Rojinegro Estructurado (Red-Black Tree) - Fondo Blanco", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 1024, 720, NULL, NULL, hInst, NULL);
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) { TranslateMessage(&msg); DispatchMessage(&msg); }
    return 0;
}