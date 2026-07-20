#include <windows.h>
#include <gdiplus.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>

#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

using namespace std;

ULONG_PTR gdiplusToken;

const char** globalNames = nullptr;
float* globalXs = nullptr;
float* globalYs = nullptr;
int* globalAdjMatrix = nullptr; 

int globalNumVertices = 0;       
int maxCapacidadVertices = 500;  
int globalStartIdx = -1;          
int globalEndIdx = -1;            

int* globalBestPath = nullptr;
int globalBestPathSize = 0;

int globalDraggedNode = -1;
int globalSelectedEdgeSource = -1; 

bool modoPeatonal = false;
bool modoAutomovil = false;
bool modoBorrar = false;

int screenWidth = 1280;  
int screenHeight = 720;

void guardarGrafo() {
    ofstream archivo("mapa_datos.txt");
    if (!archivo.is_open()) return;

    archivo << globalNumVertices << "\n";
    for (int i = 0; i < globalNumVertices; ++i) {
        archivo << *(globalXs + i) << "\n" << *(globalYs + i) << "\n" << *(globalNames + i) << "\n";
    }
    for (int i = 0; i < globalNumVertices; ++i) {
        for (int j = 0; j < globalNumVertices; ++j) {
            archivo << *(globalAdjMatrix + (i * maxCapacidadVertices) + j) << " ";
        }
        archivo << "\n";
    }
    archivo.close();
    cout << "-> [AUTO-GUARDADO] Rutas actualizadas en 'mapa_datos.txt'" << endl;
}

void cargarGrafo() {
    ifstream archivo("mapa_datos.txt");
    if (!archivo.is_open()) {
        cout << "-> No se encontro un guardado previo. Iniciando lienzo limpio." << endl;
        return;
    }

    if (!(archivo >> globalNumVertices)) {
        globalNumVertices = 0;
        return;
    }
    
    for (int i = 0; i < globalNumVertices; ++i) {
        float x, y;
        archivo >> x;
        archivo >> y;
        archivo.ignore(); 
        
        char* buffer = (char*)malloc(64 * sizeof(char));
        archivo.getline(buffer, 64);
        
        *(globalXs + i) = x;
        *(globalYs + i) = y;
        *(globalNames + i) = buffer;
    }

    for (int i = 0; i < globalNumVertices; ++i) {
        for (int j = 0; j < globalNumVertices; ++j) {
            int tipoRuta;
            archivo >> tipoRuta;
            *(globalAdjMatrix + (i * maxCapacidadVertices) + j) = tipoRuta;
        }
    }
    archivo.close();
    cout << "-> [CARGA] Se restauraron " << globalNumVertices << " puntos y sus respectivas rutas." << endl;
}

void calculateBestRoute() {
    globalBestPathSize = 0;
    if (globalStartIdx == -1 || globalEndIdx == -1 || globalStartIdx >= globalNumVertices || globalEndIdx >= globalNumVertices) return;
    if (globalStartIdx == globalEndIdx) {
        *(globalBestPath + 0) = globalStartIdx;
        globalBestPathSize = 1;
        return;
    }

    int* queue = (int*)malloc(maxCapacidadVertices * sizeof(int));
    bool* visited = (bool*)malloc(maxCapacidadVertices * sizeof(bool));
    int* parent = (int*)malloc(maxCapacidadVertices * sizeof(int));

    for (int i = 0; i < globalNumVertices; ++i) {
        *(visited + i) = false;
        *(parent + i) = -1;
    }

    int head = 0, tail = 0;
    *(queue + tail) = globalStartIdx;
    tail++;
    *(visited + globalStartIdx) = true;

    while (head < tail) {
        int current = *(queue + head);
        head++;

        if (current == globalEndIdx) break;

        for (int i = 0; i < globalNumVertices; ++i) {
            int tipoConexion = *(globalAdjMatrix + (current * maxCapacidadVertices) + i);
            if (tipoConexion > 0 && !(*(visited + i))) {
                *(visited + i) = true;
                *(parent + i) = current;
                *(queue + tail) = i;
                tail++;
            }
        }
    }

    if (*(visited + globalEndIdx)) {
        int tempPathSize = 0;
        int curr = globalEndIdx;
        while (curr != -1) {
            *(queue + tempPathSize) = curr;
            tempPathSize++;
            curr = *(parent + curr);
        }
        for (int i = 0; i < tempPathSize; ++i) {
            *(globalBestPath + i) = *(queue + (tempPathSize - 1 - i));
        }
        globalBestPathSize = tempPathSize;
    }

    free(queue); free(visited); free(parent);
}

int getClickedNode(int mouseX, int mouseY) {
    float maxDistance = 15.0f; 
    for (int i = 0; i < globalNumVertices; ++i) {
        float dx = (float)mouseX - *(globalXs + i);
        float dy = (float)mouseY - *(globalYs + i);
        if ((dx * dx + dy * dy) <= (maxDistance * maxDistance)) return i;
    }
    return -1;
}

void addEdge(int* adjMatrix, int numVerticesLimit, int src, int dest, int tipoRuta) {
    *(adjMatrix + (src * numVerticesLimit) + dest) = tipoRuta;
    *(adjMatrix + (dest * numVerticesLimit) + src) = tipoRuta;
}

void crearNuevoNodoEnMapa(int x, int y) {
    if (globalNumVertices >= maxCapacidadVertices) return;

    int nuevoIdx = globalNumVertices;
    char* bufferNombre = (char*)malloc(64 * sizeof(char));
    
    cout << "\n========================================" << endl;
    cout << "[NUEVA INTERSECCION] Nombre (X=" << x << ", Y=" << y << "): ";
    cin.clear();
    cin.sync();
    cin.getline(bufferNombre, 64);

    if (strlen(bufferNombre) == 0) {
        snprintf(bufferNombre, 64, "Punto_%d", nuevoIdx);
    }
    
    *(globalNames + nuevoIdx) = bufferNombre;
    *(globalXs + nuevoIdx) = (float)x;
    *(globalYs + nuevoIdx) = (float)y;

    globalNumVertices++;
    guardarGrafo(); 
    InvalidateRect(GetActiveWindow(), nullptr, TRUE);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_SIZE:
            screenWidth = LOWORD(lParam);
            screenHeight = HIWORD(lParam);
            InvalidateRect(hwnd, nullptr, TRUE);
            break;
        case WM_KEYDOWN:
            if (wParam == 'P' || wParam == 'p') modoPeatonal = true;
            if (wParam == 'A' || wParam == 'a') modoAutomovil = true;
            if (wParam == 'E' || wParam == 'e') modoBorrar = true;
            break;
        case WM_KEYUP:
            if (wParam == 'P' || wParam == 'p') { modoPeatonal = false; globalSelectedEdgeSource = -1; InvalidateRect(hwnd, nullptr, TRUE); }
            if (wParam == 'A' || wParam == 'a') { modoAutomovil = false; globalSelectedEdgeSource = -1; InvalidateRect(hwnd, nullptr, TRUE); }
            if (wParam == 'E' || wParam == 'e') { modoBorrar = false; globalSelectedEdgeSource = -1; InvalidateRect(hwnd, nullptr, TRUE); }
            break;
        case WM_LBUTTONDOWN: {
            int mouseX = LOWORD(lParam);
            int mouseY = HIWORD(lParam);

            if (LOWORD(wParam) & MK_CONTROL) {
                crearNuevoNodoEnMapa(mouseX, mouseY);
                calculateBestRoute();
                InvalidateRect(hwnd, nullptr, TRUE);
                break;
            }

            int clicked = getClickedNode(mouseX, mouseY);
            if (clicked != -1) {
                if (LOWORD(wParam) & MK_SHIFT) {
                    globalStartIdx = clicked;
                    calculateBestRoute();
                    InvalidateRect(hwnd, nullptr, TRUE);
                } 
                else if (modoPeatonal || modoAutomovil) {
                    int tipoAAsignar = modoPeatonal ? 1 : 2;
                    if (globalSelectedEdgeSource == -1) {
                        globalSelectedEdgeSource = clicked;
                        InvalidateRect(hwnd, nullptr, TRUE);
                    } else {
                        if (globalSelectedEdgeSource != clicked) {
                            addEdge(globalAdjMatrix, maxCapacidadVertices, globalSelectedEdgeSource, clicked, tipoAAsignar);
                            globalSelectedEdgeSource = clicked; 
                            calculateBestRoute();
                            guardarGrafo(); 
                            InvalidateRect(hwnd, nullptr, TRUE);
                        }
                    }
                } 
                else {
                    globalDraggedNode = clicked;
                    SetCapture(hwnd);
                }
            }
        }
        break;
        case WM_MOUSEMOVE:
            if (globalDraggedNode != -1) {
                *(globalXs + globalDraggedNode) = (float)LOWORD(lParam);
                *(globalYs + globalDraggedNode) = (float)HIWORD(lParam);
                InvalidateRect(hwnd, nullptr, TRUE);
            }
            break;
        case WM_LBUTTONUP:
            if (globalDraggedNode != -1) {
                globalDraggedNode = -1;
                ReleaseCapture();
                calculateBestRoute();
                guardarGrafo(); 
                InvalidateRect(hwnd, nullptr, TRUE);
            }
            break;
        case WM_RBUTTONDOWN: {
            int mouseX = LOWORD(lParam);
            int mouseY = HIWORD(lParam);
            int clicked = getClickedNode(mouseX, mouseY);
            
            if (clicked != -1) {
                if (modoBorrar) {
                    if (globalSelectedEdgeSource == -1) {
                        globalSelectedEdgeSource = clicked;
                        InvalidateRect(hwnd, nullptr, TRUE);
                    } else {
                        if (globalSelectedEdgeSource != clicked) {
                            // 0 significa desconectado o removido
                            *(globalAdjMatrix + (globalSelectedEdgeSource * maxCapacidadVertices) + clicked) = 0;
                            *(globalAdjMatrix + (clicked * maxCapacidadVertices) + globalSelectedEdgeSource) = 0;
                            
                            cout << "-> [ELIMINADO] Tramo removido entre los puntos seleccionados." << endl;
                            globalSelectedEdgeSource = -1; 
                            calculateBestRoute();
                            guardarGrafo(); 
                            InvalidateRect(hwnd, nullptr, TRUE);
                        }
                    }
                }
                else if (LOWORD(wParam) & MK_SHIFT) {
                    globalEndIdx = clicked;
                    calculateBestRoute();
                    InvalidateRect(hwnd, nullptr, TRUE);
                }
            }
        }
        break;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            Gdiplus::Graphics graphics(hdc);
            
            graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
            bool backgroundRendered = false;

            Gdiplus::Image localImage(L"mapa_espe.png");
            if (localImage.GetLastStatus() == Gdiplus::Ok) {
                localImage.RotateFlip(Gdiplus::Rotate90FlipNone);
                graphics.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBilinear);
                graphics.DrawImage(&localImage, 0, 0, screenWidth, screenHeight);
                backgroundRendered = true;
            }

            if (!backgroundRendered) {
                HBRUSH hBackground = CreateSolidBrush(RGB(40, 40, 40));
                FillRect(hdc, &ps.rcPaint, hBackground); DeleteObject(hBackground);
            }
            Gdiplus::Pen penPeatonal(Gdiplus::Color(200, 52, 152, 219), 2.5f); 
            Gdiplus::Pen penAutomovil(Gdiplus::Color(220, 41, 128, 185), 5.5f);

            for (int i = 0; i < globalNumVertices; ++i) {
                for (int j = i; j < globalNumVertices; ++j) {
                    int tipoRuta = *(globalAdjMatrix + (i * maxCapacidadVertices) + j);
                    if (tipoRuta == 1) {
                        graphics.DrawLine(&penPeatonal, *(globalXs + i), *(globalYs + i), *(globalXs + j), *(globalYs + j));
                    } else if (tipoRuta == 2) {
                        graphics.DrawLine(&penAutomovil, *(globalXs + i), *(globalYs + i), *(globalXs + j), *(globalYs + j));
                    }
                }
            }

            if (globalBestPathSize > 1) {
                Gdiplus::Pen penRoute(Gdiplus::Color(255, 39, 174, 96), 6.5f); 
                for (int i = 0; i < globalBestPathSize - 1; ++i) {
                    int src = *(globalBestPath + i);
                    int dest = *(globalBestPath + i + 1);
                    graphics.DrawLine(&penRoute, *(globalXs + src), *(globalYs + src), *(globalXs + dest), *(globalYs + dest));
                }
            }

            for (int i = 0; i < globalNumVertices; ++i) {
                float x = *(globalXs + i); float y = *(globalYs + i);
                const char* name = *(globalNames + i);

                Gdiplus::Color colorNodo(255, 155, 89, 182); 
                
                if (i == globalSelectedEdgeSource) {
                    colorNodo = Gdiplus::Color(255, 241, 196, 15);
                } else if (i == globalStartIdx) {
                    colorNodo = Gdiplus::Color(255, 46, 204, 113);
                } else if (i == globalEndIdx) {
                    colorNodo = Gdiplus::Color(255, 231, 76, 60);
                }

                Gdiplus::SolidBrush brushNode(colorNodo);
                Gdiplus::Pen penBorder(Gdiplus::Color(255, 255, 255, 255), 1.0f);

                float r = 4.0f; 
                graphics.FillEllipse(&brushNode, x - r, y - r, r * 2, r * 2);
                graphics.DrawEllipse(&penBorder, x - r, y - r, r * 2, r * 2);

                if (strstr(name, "Punto") == nullptr) {
                    SetTextColor(hdc, RGB(255, 255, 255)); SetBkMode(hdc, OPAQUE); SetBkColor(hdc, RGB(44, 62, 80));
                    int len = 0; while (*(name + len) != '\0') len++;
                    TextOutA(hdc, (int)x - (len * 3), (int)y + 8, name, len);
                }
            }

            EndPaint(hwnd, &ps);
        }
        break;
        case WM_DESTROY: 
            PostQuitMessage(0); 
            break;
        default: return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int main() {
    AllocConsole(); 
    freopen("CONOUT$", "w", stdout); freopen("CONIN$", "r", stdin);

    Gdiplus::GdiplusStartupInput gdiplusStartupInput; 
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    globalNames = (const char**)malloc(maxCapacidadVertices * sizeof(const char*));
    globalXs = (float*)malloc(maxCapacidadVertices * sizeof(float));
    globalYs = (float*)malloc(maxCapacidadVertices * sizeof(float));
    globalAdjMatrix = (int*)malloc(maxCapacidadVertices * maxCapacidadVertices * sizeof(int));
    globalBestPath = (int*)malloc(maxCapacidadVertices * sizeof(int));

    for (int i = 0; i < maxCapacidadVertices * maxCapacidadVertices; ++i) *(globalAdjMatrix + i) = 0;
    cargarGrafo();

    HINSTANCE hInstance = GetModuleHandle(nullptr);
    WNDCLASSEXA wc = {}; wc.cbSize = sizeof(WNDCLASSEXA); wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance; wc.lpszClassName = "ESPE_MultiRoad_Engine";
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW); wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    
    RegisterClassExA(&wc);
    HWND hwnd = CreateWindowExA(0, "ESPE_MultiRoad_Engine", "Constructor ESPE - Rutas Peatonales y Vehiculares",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, nullptr, nullptr, hInstance, nullptr);

    ShowWindow(hwnd, SW_SHOWMAXIMIZED); UpdateWindow(hwnd);

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) { TranslateMessage(&msg); DispatchMessage(&msg); }

    free(globalNames); free(globalXs); free(globalYs); free(globalAdjMatrix); free(globalBestPath);
    Gdiplus::GdiplusShutdown(gdiplusToken);
    return 0;
}