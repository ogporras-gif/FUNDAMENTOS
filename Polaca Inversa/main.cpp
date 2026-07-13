#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include <limits>
#include <stack>

using namespace std;

const double PI = 3.14159265358979323846;

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
    if (op == "*" || op == "x") return a * b;
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

double evaluarRPN(const string& expresion) {
    stack<double> pila;
    stringstream ss(expresion);
    string token;

    while (ss >> token) {
        double numeroConvertido = 0;
        if (esOperadorBinario(token)) {
            if (pila.size() < 2) throw runtime_error("Error de sintaxis en RPN.");
            double operando2 = pila.top(); pila.pop();
            double operando1 = pila.top(); pila.pop();
            pila.push(aplicarOperacionBinaria(operando1, operando2, token));
        } 
        else if (esFuncionUnaria(token)) {
            if (pila.empty()) throw runtime_error("Error de sintaxis en RPN.");
            double operando = pila.top(); pila.pop();
            pila.push(aplicarFuncionUnaria(operando, token));
        } 
        else if (convertirANumero(token, numeroConvertido)) {
            pila.push(numeroConvertido);
        }
    }
    if (pila.size() != 1) throw runtime_error("Error de sintaxis en RPN.");
    return pila.top();
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
                
                double resultado = evaluarRPN(rpn);
                cout << ">>> Resultado Numerico Final: " << resultado << "\n\n";
            } 
            catch (const runtime_error& e) {
                cout << "\n[ERROR] " << e.what() << "\n\n";
            }
        }
    } while (opcion != 2);

    return 0;
}