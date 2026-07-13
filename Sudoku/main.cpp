#include <windows.h>
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>

using namespace std;

vector<int> sudoku = {
    5, 3, 0, 0, 7, 0, 0, 0, 0,
    6, 0, 0, 1, 9, 5, 0, 0, 0,
    0, 9, 8, 0, 0, 0, 0, 6, 0,
    8, 0, 0, 0, 6, 0, 0, 0, 3,
    4, 0, 0, 8, 0, 3, 0, 0, 1,
    7, 0, 0, 0, 2, 0, 0, 0, 6,
    0, 6, 0, 0, 0, 0, 2, 8, 0,
    0, 0, 0, 4, 1, 9, 0, 0, 5,
    0, 0, 0, 0, 8, 0, 0, 7, 9
};

vector<bool> es_pista_original(81, false);

CRITICAL_SECTION cs;

#define ID_BOTON_SOLVE   1
#define ID_BOTON_REINICIAR 2

HWND hVentanaPrincipal;
bool resolviendo = false;
bool EsSeguro(int fila, int col, int num) {
    for (int x = 0; x < 9; x++) {
        if (sudoku[fila * 9 + x] == num) return false;
    }
    for (int x = 0; x < 9; x++) {
        if (sudoku[x * 9 + col] == num) return false;
    }
    int inicioFila = fila - fila % 3;
    int inicioCol = col - col % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (sudoku[(i + inicioFila) * 9 + (j + inicioCol)] == num) return false;
        }
    }
    return true;
}
bool ResolverSudoku() {
    int fila = -1;
    int col = -1;
    bool vacio = true;
    
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (sudoku[i * 9 + j] == 0) {
                fila = i;
                col = j;
                vacio = false;
                break;
            }
        }
        if (!vacio) break;
    }

    if (vacio) return true;

    for (int num = 1; num <= 9; num++) {
        if (EsSeguro(fila, col, num)) {
            sudoku[fila * 9 + col] = num;
            
            if (ResolverSudoku()) return true;

            sudoku[fila * 9 + col] = 0;
        }
    }
    return false;
}

void InicializarPistas() {
    for (int i = 0; i < 81; i++) {
        if (sudoku[i] != 0) es_pista_original[i] = true;
        else es_pista_original[i] = false;
    }
}

// --- HILOS ASÍNCRONOS ---
DWORD WINAPI HiloResolver(LPVOID lpParam) {
    resolviendo = true;
    EnterCriticalSection(&cs);
    
    ResolverSudoku();
    
    LeaveCriticalSection(&cs);
    resolviendo = false;
    InvalidateRect(hVentanaPrincipal, NULL, FALSE);
    return 0;
}

DWORD WINAPI HiloReiniciar(LPVOID lpParam) {
    EnterCriticalSection(&cs);
    vector<int> inicial = {
        5, 3, 0, 0, 7, 0, 0, 0, 0,
        6, 0, 0, 1, 9, 5, 0, 0, 0,
        0, 9, 8, 0, 0, 0, 0, 6, 0,
        8, 0, 0, 0, 6, 0, 0, 0, 3,
        4, 0, 0, 8, 0, 3, 0, 0, 1,
        7, 0, 0, 0, 2, 0, 0, 0, 6,
        0, 6, 0, 0, 0, 0, 2, 8, 0,
        0, 0, 0, 4, 1, 9, 0, 0, 5,
        0, 0, 0, 0, 8, 0, 0, 7, 9
    };
    sudoku = inicial;
    LeaveCriticalSection(&cs);
    InvalidateRect(hVentanaPrincipal, NULL, FALSE);
    return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_CREATE:
            CreateWindowW(L"BUTTON", L"Solve ✨", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 25, 20, 120, 35, hwnd, (HMENU)ID_BOTON_SOLVE, NULL, NULL);
            CreateWindowW(L"BUTTON", L"Reset 🔄", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 160, 20, 120, 35, hwnd, (HMENU)ID_BOTON_REINICIAR, NULL, NULL);
            InicializarPistas();
            break;
            
        case WM_COMMAND:
            if (resolviendo) break;
            
            if (LOWORD(wParam) == ID_BOTON_SOLVE) {
                CreateThread(NULL, 0, HiloResolver, NULL, 0, NULL);
            }
            else if (LOWORD(wParam) == ID_BOTON_REINICIAR) {
                CreateThread(NULL, 0, HiloReiniciar, NULL, 0, NULL);
            }
            break;
            
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
        
            RECT rect; GetClientRect(hwnd, &rect);
            HDC hdcMem = CreateCompatibleDC(hdc);
            HBITMAP hbmMem = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
            HGDIOBJ hOldObj = SelectObject(hdcMem, hbmMem);
            
            HBRUSH fondo = CreateSolidBrush(RGB(245, 246, 250));
            FillRect(hdcMem, &rect, fondo); DeleteObject(fondo);
            
            EnterCriticalSection(&cs);
            
            int tam_celda = 42;
            int ox = (rect.right - (9 * tam_celda)) / 2;
            int oy = 85; 
            
            for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 9; j++) {
                    int valor = sudoku[i * 9 + j];
                    HBRUSH hBrush;
                    if (es_pista_original[i * 9 + j]) hBrush = CreateSolidBrush(RGB(255, 255, 255));
                    else hBrush = CreateSolidBrush(RGB(224, 242, 241));
                    
                    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(189, 195, 199));
                    
                    SelectObject(hdcMem, hBrush);
                    SelectObject(hdcMem, hPen);
                    
                    int x1 = ox + j * tam_celda;
                    int y1 = oy + i * tam_celda;
                    int x2 = x1 + tam_celda;
                    int y2 = y1 + tam_celda;
                    
                    Rectangle(hdcMem, x1, y1, x2, y2);
                    if (valor > 0) {
                        SetBkMode(hdcMem, TRANSPARENT);
                        if (es_pista_original[i * 9 + j]) SetTextColor(hdcMem, RGB(44, 62, 80));
                        else SetTextColor(hdcMem, RGB(26, 82, 118));
                        
                        HFONT hFont = CreateFontW(22, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
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
            HPEN hPenGrueso = CreatePen(PS_SOLID, 3, RGB(44, 62, 80));
            SelectObject(hdcMem, hPenGrueso);
            
            for (int i = 0; i <= 9; i += 3) {
                MoveToEx(hdcMem, ox, oy + i * tam_celda, NULL);
                LineTo(hdcMem, ox + 9 * tam_celda, oy + i * tam_celda);
                MoveToEx(hdcMem, ox + i * tam_celda, oy, NULL);
                LineTo(hdcMem, ox + i * tam_celda, oy + 9 * tam_celda);
            }
            DeleteObject(hPenGrueso);
            
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
    wc.lpszClassName = L"MiClaseSudokuSolver";
    wc.lpfnWndProc = WindowProcedure;
    
    if (!RegisterClassW(&wc)) return -1;
    
    hVentanaPrincipal = CreateWindowW(L"MiClaseSudokuSolver", L"Sudoku Solver - Backtracking en Vectores Paralelos", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 460, 520, NULL, NULL, hInst, NULL);
    
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    DeleteCriticalSection(&cs);
    return 0;
}