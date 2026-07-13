#include <windows.h>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

const int MAX_KEYS = 2; 

vector<vector<int>> claves; 
vector<vector<int>> hijos;  
vector<bool> es_hoja;       
int raiz = -1;

CRITICAL_SECTION cs;

#define ID_BOTON_INSERTAR 1
#define ID_CAJA_NUMERO    2
#define ID_BOTON_BORRAR   3

HWND hCajaNumero;
HWND hVentanaPrincipal;

int CrearNuevoNodo(bool hoja) {
    claves.push_back(vector<int>());
    hijos.push_back(vector<int>());
    es_hoja.push_back(hoja);
    return (int)claves.size() - 1;
}
void SplitHijo(int padre_idx, int i, int hijo_idx) {
    int z = CrearNuevoNodo(es_hoja[hijo_idx]);

    claves[z].push_back(claves[hijo_idx][1]);
    claves[hijo_idx].pop_back();

    if (!es_hoja[hijo_idx]) {
        hijos[z].push_back(hijos[hijo_idx][2]);
        hijos[hijo_idx].pop_back();
    }

    hijos[padre_idx].insert(hijos[padre_idx].begin() + i + 1, z);
    claves[padre_idx].insert(claves[padre_idx].begin() + i, claves[hijo_idx][0]);
    claves[hijo_idx].erase(claves[hijo_idx].begin());
    
    if (!es_hoja[hijo_idx]) {
        hijos[z].insert(hijos[z].begin(), hijos[hijo_idx][1]);
        hijos[hijo_idx].pop_back();
    }
}

void InsertarNoLleno(int nodo_idx, int k) {
    int i = (int)claves[nodo_idx].size() - 1;

    if (es_hoja[nodo_idx]) {
        claves[nodo_idx].push_back(0);
        while (i >= 0 && claves[nodo_idx][i] > k) {
            claves[nodo_idx][i + 1] = claves[nodo_idx][i];
            i--;
        }
        claves[nodo_idx][i + 1] = k;
    } else {
        while (i >= 0 && claves[nodo_idx][i] > k) i--;
        i++;
        if (claves[hijos[nodo_idx][i]].size() == MAX_KEYS) {
            SplitHijo(nodo_idx, i, hijos[nodo_idx][i]);
            if (claves[nodo_idx][i] < k) i++;
        }
        InsertarNoLleno(hijos[nodo_idx][i], k);
    }
}

void Insertar(int valor) {
    EnterCriticalSection(&cs);
    if (raiz == -1) {
        raiz = CrearNuevoNodo(true);
        claves[raiz].push_back(valor);
    } else {
        if (claves[raiz].size() == MAX_KEYS) {
            int s = CrearNuevoNodo(false);
            hijos[s].push_back(raiz);
            SplitHijo(s, 0, raiz);
            int i = 0;
            if (claves[s][0] < valor) i++;
            InsertarNoLleno(hijos[s][i], valor);
            raiz = s;
        } else {
            InsertarNoLleno(raiz, valor);
        }
    }
    LeaveCriticalSection(&cs);
    InvalidateRect(hVentanaPrincipal, NULL, FALSE);
}

int EncontrarClave(int nodo_idx, int k) {
    int idx = 0;
    while (idx < (int)claves[nodo_idx].size() && claves[nodo_idx][idx] < k) idx++;
    return idx;
}

void Fusionar(int nodo_idx, int idx) {
    int hijo = hijos[nodo_idx][idx];
    int hermano = hijos[nodo_idx][idx + 1];

    claves[hijo].push_back(claves[nodo_idx][idx]);

    for (int i = 0; i < (int)claves[hermano].size(); ++i) {
        claves[hijo].push_back(claves[hermano][i]);
    }
    if (!es_hoja[hijo]) {
        for (int i = 0; i < (int)hijos[hermano].size(); ++i) {
            hijos[hijo].push_back(hijos[hermano][i]);
        }
    }

    claves[nodo_idx].erase(claves[nodo_idx].begin() + idx);
    hijos[nodo_idx].erase(hijos[nodo_idx].begin() + idx + 1);
}

void PrestarPrevio(int nodo_idx, int idx) {
    int hijo = hijos[nodo_idx][idx];
    int hermano = hijos[nodo_idx][idx - 1];

    claves[hijo].insert(claves[hijo].begin(), claves[nodo_idx][idx - 1]);
    if (!es_hoja[hijo]) {
        hijos[hijo].insert(hijos[hijo].begin(), hijos[hermano].back());
    }

    claves[nodo_idx][idx - 1] = claves[hermano].empty() ? 0 : claves[hermano].back();
    claves[hermano].pop_back();
    if (!es_hoja[hermano]) hijos[hermano].pop_back();
}

void PrestarSiguiente(int nodo_idx, int idx) {
    int hijo = hijos[nodo_idx][idx];
    int hermano = hijos[nodo_idx][idx + 1];

    claves[hijo].push_back(claves[nodo_idx][idx]);
    claves[nodo_idx][idx] = claves[hermano][0];
    claves[hermano].erase(claves[hermano].begin());

    if (!es_hoja[hijo]) {
        hijos[hijo].push_back(hijos[hermano][0]);
        hijos[hermano].erase(hijos[hermano].begin());
    }
}

void LlenarHijo(int nodo_idx, int idx) {
    if (idx != 0 && claves[hijos[nodo_idx][idx - 1]].size() >= 1) {
        PrestarPrevio(nodo_idx, idx);
    } else if (idx != (int)claves[nodo_idx].size() && claves[hijos[nodo_idx][idx + 1]].size() >= 1) {
        PrestarSiguiente(nodo_idx, idx);
    } else {
        if (idx != (int)claves[nodo_idx].size()) Fusionar(nodo_idx, idx);
        else Fusionar(nodo_idx, idx - 1);
    }
}

int GetPred(int nodo_idx, int idx) {
    int curr = hijos[nodo_idx][idx];
    while (!es_hoja[curr]) curr = hijos[curr].back();
    return claves[curr].back();
}

int GetSuc(int nodo_idx, int idx) {
    int curr = hijos[nodo_idx][idx + 1];
    while (!es_hoja[curr]) curr = hijos[curr][0];
    return claves[curr][0];
}

void EliminarDeNodo(int nodo_idx, int k) {
    int idx = EncontrarClave(nodo_idx, k);

    if (idx < (int)claves[nodo_idx].size() && claves[nodo_idx][idx] == k) {
        if (es_hoja[nodo_idx]) {
            claves[nodo_idx].erase(claves[nodo_idx].begin() + idx);
        } else {
            if (claves[hijos[nodo_idx][idx]].size() >= 1) {
                int pred = GetPred(nodo_idx, idx);
                claves[nodo_idx][idx] = pred;
                EliminarDeNodo(hijos[nodo_idx][idx], pred);
            } else if (claves[hijos[nodo_idx][idx + 1]].size() >= 1) {
                int suc = GetSuc(nodo_idx, idx);
                claves[nodo_idx][idx] = suc;
                EliminarDeNodo(hijos[nodo_idx][idx + 1], suc);
            } else {
                Fusionar(nodo_idx, idx);
                EliminarDeNodo(hijos[nodo_idx][idx], k);
            }
        }
    } else {
        if (es_hoja[nodo_idx]) return;

        bool bandera = (idx == (int)claves[nodo_idx].size());
        if (claves[hijos[nodo_idx][idx]].size() < 1) {
            LlenarHijo(nodo_idx, idx);
        }

        if (bandera && idx > (int)claves[nodo_idx].size()) {
            EliminarDeNodo(hijos[nodo_idx][idx - 1], k);
        } else {
            EliminarDeNodo(hijos[nodo_idx][idx], k);
        }
    }
}

void Borrar(int valor) {
    if (raiz == -1) return;
    EnterCriticalSection(&cs);
    EliminarDeNodo(raiz, valor);
    if (claves[raiz].empty()) {
        if (es_hoja[raiz]) raiz = -1;
        else raiz = hijos[raiz][0];
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


void DibujarArbolB(HDC hdc, int nodo_idx, float x, float y, float desp_x) {
    if (nodo_idx == -1 || nodo_idx >= (int)claves.size()) return;

    int num_claves = (int)claves[nodo_idx].size();
    int ancho_celda = 34;
    int alto_nodo = 30;
    int ancho_total = num_claves * ancho_celda;
    int inicio_x = (int)(x - (ancho_total / 2.0f));

    if (!es_hoja[nodo_idx]) {
        for (int i = 0; i <= num_claves; i++) {
            if (i < (int)hijos[nodo_idx].size() && hijos[nodo_idx][i] != -1) {
                HPEN hPen = CreatePen(PS_SOLID, 2, RGB(140, 140, 140));
                SelectObject(hdc, hPen);
                MoveToEx(hdc, inicio_x + (i * ancho_celda), (int)(y + alto_nodo), NULL);
                
                float delta_hijo = x + ((i - (num_claves / 2.0f)) * desp_x * 1.5f);
                LineTo(hdc, (int)delta_hijo, (int)(y + 90));
                DeleteObject(hPen);

                DibujarArbolB(hdc, hijos[nodo_idx][i], delta_hijo, y + 90, desp_x * 0.45f);
            }
        }
    }

    HBRUSH hBrush = CreateSolidBrush(RGB(230, 126, 34));
    HPEN hPenNodo = CreatePen(PS_SOLID, 2, RGB(211, 84, 0));
    SelectObject(hdc, hBrush);
    SelectObject(hdc, hPenNodo);

    Rectangle(hdc, inicio_x, (int)y, inicio_x + ancho_total, (int)(y + alto_nodo));
    
    DeleteObject(hBrush);
    DeleteObject(hPenNodo);

    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(255, 255, 255));
    HPEN hPenLinea = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
    SelectObject(hdc, hPenLinea);

    for (int i = 0; i < num_claves; i++) {
        if (i > 0) {
            MoveToEx(hdc, inicio_x + (i * ancho_celda), (int)y, NULL);
            LineTo(hdc, inicio_x + (i * ancho_celda), (int)(y + alto_nodo));
        }
        wstringstream wss; 
        wss << claves[nodo_idx][i];
        wstring txt = wss.str();
        RECT rectCelda = { inicio_x + (i * ancho_celda), (int)y, inicio_x + ((i + 1) * ancho_celda), (int)(y + alto_nodo) };
        DrawTextW(hdc, txt.c_str(), -1, &rectCelda, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
    DeleteObject(hPenLinea);
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_CREATE:
            CreateWindowW(L"STATIC", L"Número:", WS_CHILD | WS_VISIBLE, 20, 18, 65, 20, hwnd, NULL, NULL, NULL);
            hCajaNumero = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, 90, 15, 80, 25, hwnd, (HMENU)ID_CAJA_NUMERO, NULL, NULL);
            CreateWindowW(L"BUTTON", L"Insertar B", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 185, 13, 100, 28, hwnd, (HMENU)ID_BOTON_INSERTAR, NULL, NULL);
            CreateWindowW(L"BUTTON", L"Borrar B", WS_CHILD | WS_VISIBLE, 300, 13, 100, 28, hwnd, (HMENU)ID_BOTON_BORRAR, NULL, NULL);
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
            if (raiz != -1) { 
                RECT rect; GetClientRect(hwnd, &rect); 
                DibujarArbolB(hdc, raiz, rect.right / 2.0f, 130.0f, rect.right / 5.0f); 
            }
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
    wc.hInstance = hInst; wc.lpszClassName = L"MiClaseArbolBDef"; wc.lpfnWndProc = WindowProcedure;
    if (!RegisterClassW(&wc)) return -1;
    hVentanaPrincipal = CreateWindowW(L"MiClaseArbolBDef", L"Árbol B de Orden 3 (2-3) - Totalmente Estable", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 1024, 720, NULL, NULL, hInst, NULL);
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) { TranslateMessage(&msg); DispatchMessage(&msg); }
    return 0;
}