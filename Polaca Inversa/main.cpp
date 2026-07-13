#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include <limits>
#include <stack>
#include <stdexcept>

using namespace std;

bool esOperadorBinario(const string& token) {
    return token == "+" || token == "-" || token == "*" || token == "/" || token == "^";
}

bool esFuncionUnaria(const string& token) {
    return token == "sen"  || token == "cos"  || token == "tan"  ||
           token == "cosec"|| token == "sec"  || token == "cotan"||
           token == "sqrt" || token == "log"  || token == "ln";
}

bool convertirANumero(const string& token, double& numeroValor) {
    stringstream ss(token);
    ss.imbue(locale::classic()); 
    char c;
    return (ss >> numeroValor) && !(ss >> c);
}

int obtenerPrecedencia(const string& op) {
    if (op == "+" || op == "-") return 1;
    if (op == "*" || op == "/") return 2;
    if (op == "^") return 3;
    if (esFuncionUnaria(op)) return 4;
    return 0;
}

string convertirInfijoARPN(const string& infijo) {
    stack<string> pilaOperadores;
    stringstream ss(infijo);
    stringstream resultadoRPN;
    string token;
    bool primerToken = true;

    while (ss >> token) {
        double num;
        if (convertirANumero(token, num)) {
            if (!primerToken) resultadoRPN << " ";
            resultadoRPN << token;
            primerToken = false;
        } 
        else if (esFuncionUnaria(token) || token == "(") {
            pilaOperadores.push(token);
        } 
        else if (token == ")") {
            while (!pilaOperadores.empty() && pilaOperadores.top() != "(") {
                if (!primerToken) resultadoRPN << " ";
                resultadoRPN << pilaOperadores.top();
                pilaOperadores.pop();
                primerToken = false;
            }
            if (pilaOperadores.empty()) {
                throw runtime_error("Error de sintaxis: Parentesis desbalanceados.");
            }
            pilaOperadores.pop();
            
            if (!pilaOperadores.empty() && esFuncionUnaria(pilaOperadores.top())) {
                if (!primerToken) resultadoRPN << " ";
                resultadoRPN << pilaOperadores.top();
                pilaOperadores.pop();
                primerToken = false;
            }
        } 
        else if (esOperadorBinario(token)) {
            while (!pilaOperadores.empty() && 
                   (obtenerPrecedencia(pilaOperadores.top()) > obtenerPrecedencia(token) ||
                   (obtenerPrecedencia(pilaOperadores.top()) == obtenerPrecedencia(token) && token != "^"))) {
                if (!primerToken) resultadoRPN << " ";
                resultadoRPN << pilaOperadores.top();
                pilaOperadores.pop();
                primerToken = false;
            }
            pilaOperadores.push(token);
        } 
        else {
            throw runtime_error("Token desconocido: " + token);
        }
    }

    while (!pilaOperadores.empty()) {
        if (pilaOperadores.top() == "(" || pilaOperadores.top() == ")") {
            throw runtime_error("Error de sintaxis: Parentesis desbalanceados.");
        }
        if (!primerToken) resultadoRPN << " ";
        resultadoRPN << pilaOperadores.top();
        pilaOperadores.pop();
        primerToken = false;
    }

    return resultadoRPN.str();
}

double aplicarOperacionBinaria(double a, double b, const string& op) {
    if (op == "+") return a + b;
    if (op == "-") return a - b;
    if (op == "*") return a * b;
    if (op == "/") {
        if (b == 0) throw runtime_error("Error matematico: Division por cero.");
        return a / b;
    }
    if (op == "^") return pow(a, b);
    return 0;
}

double aplicarFuncionUnaria(double x, const string& func) {
    if (func == "sen")   return sin(x);
    if (func == "cos")   return cos(x);
    if (func == "tan")   return tan(x);
    if (func == "sqrt") {
        if (x < 0) throw runtime_error("Error matematico: Raiz cuadrada negativa.");
        return sqrt(x);
    }
    return 0;
}

class Nodo {
public:
    string dato;
    Nodo* izquierdo;
    Nodo* derecho;
    Nodo(string val) {
        dato = val;
        izquierdo = nullptr;
        derecho = nullptr;
    }
};

Nodo* construirArbol(const string& rpn) {
    stack<Nodo*> pila;
    stringstream ss(rpn);
    string token;
    while (ss >> token) {
        Nodo* nuevo = new Nodo(token);
        if (esOperadorBinario(token)) {
            if (pila.size() < 2) throw runtime_error("Error de sintaxis en RPN.");
            nuevo->derecho = pila.top(); pila.pop();
            nuevo->izquierdo = pila.top(); pila.pop();
            pila.push(nuevo);
        } else if (esFuncionUnaria(token)) {
            if (pila.empty()) throw runtime_error("Error de sintaxis en RPN.");
            nuevo->izquierdo = pila.top(); pila.pop();
            pila.push(nuevo);
        } else {
            pila.push(nuevo);
        }
    }
    if (pila.size() != 1) throw runtime_error("Error de sintaxis en RPN.");
    return pila.top();
}

void preorden(Nodo* raiz) {
    if (raiz == nullptr) return;
    cout << raiz->dato << " ";
    preorden(raiz->izquierdo);
    preorden(raiz->derecho);
}

void inorden(Nodo* raiz) {
    if (raiz == nullptr) return;
    if (raiz->izquierdo || raiz->derecho) cout << "( ";
    inorden(raiz->izquierdo);
    cout << raiz->dato << " ";
    inorden(raiz->derecho);
    if (raiz->izquierdo || raiz->derecho) cout << ") ";
}

void postorden(Nodo* raiz) {
    if (raiz == nullptr) return;
    postorden(raiz->izquierdo);
    postorden(raiz->derecho);
    cout << raiz->dato << " ";
}

double evaluarArbol(Nodo* raiz) {
    if (raiz == nullptr) return 0;
    if (!esOperadorBinario(raiz->dato) && !esFuncionUnaria(raiz->dato)) {
        double val;
        convertirANumero(raiz->dato, val);
        return val;
    }
    if (esFuncionUnaria(raiz->dato)) {
        double valIzq = evaluarArbol(raiz->izquierdo);
        return aplicarFuncionUnaria(valIzq, raiz->dato);
    }
    double valIzq = evaluarArbol(raiz->izquierdo);
    double valDer = evaluarArbol(raiz->derecho);
    return aplicarOperacionBinaria(valIzq, valDer, raiz->dato);
}

void liberarArbol(Nodo* raiz) {
    if (raiz == nullptr) return;
    liberarArbol(raiz->izquierdo);
    liberarArbol(raiz->derecho);
    delete raiz;
}

int main() {
    int opcion;
    do {
        cout << "=== CONVERSOR INFIJO A NOTACION POLACA INVERSA (RPN) ===\n";
        cout << "1. Convertir funcion (infija) a RPN y evaluar\n";
        cout << "2. Salir\n";
        cout << "Seleccione una opcion: ";
        
        if (!(cin >> opcion)) {
            cout << "\n[!] Opcion invalida.\n\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (opcion == 1) {
            string expresion;
            cout << "\n* IMPORTANTE: Separa TODOS los elementos con espacios (ej: ( 3 + 4 ) * sen ( 2 ) )\n";
            cout << "Ingrese la funcion en notacion normal:\n> ";
            getline(cin, expresion);
            
            try {
                string rpn = convertirInfijoARPN(expresion);
                cout << "\n--------------------------------------------------------\n";
                cout << ">>> Expresion en Polaca Inversa (RPN): [ " << rpn << " ]\n";
                cout << "--------------------------------------------------------\n";
                
                Nodo* raiz = construirArbol(rpn);
                
                cout << ">>> Recorrido Prefijo: ";
                preorden(raiz);
                cout << "\n";
                
                cout << ">>> Recorrido Infijo: ";
                inorden(raiz);
                cout << "\n";
                
                cout << ">>> Recorrido Posfijo: ";
                postorden(raiz);
                cout << "\n";
                
                double resultado = evaluarArbol(raiz);
                cout << ">>> Resultado Numerico Final: " << resultado << "\n\n";
                
                liberarArbol(raiz);
            } 
            catch (const runtime_error& e) {
                cout << "\n[ERROR] " << e.what() << "\n\n";
            }
        }
    } while (opcion != 2);

    return 0;
}