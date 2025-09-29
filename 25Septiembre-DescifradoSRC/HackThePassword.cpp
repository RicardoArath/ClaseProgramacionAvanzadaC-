#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <climits>
using namespace std;

bool debug = false;

void debugPrint(const string& mensaje) {
    if (debug) {
        cout << "[DEBUG] " << mensaje << endl;
    }
}

// Función para calcular la distancia mínima absoluta entre caracteres consecutivos
int calcularDistanciaMinima(const string& str) {
    int minDist = INT_MAX;
    for (int i = 0; i < str.length() - 1; i++) {
        int dist = abs(str[i] - str[i + 1]);
        minDist = min(minDist, dist);
    }
    return minDist;
}

// Función para generar la contraseña de un username
string generarContrasena(const string& username) {
    string sorted_username = username;
    sort(sorted_username.begin(), sorted_username.end());
    
    vector<string> permutaciones;
    
    // Generar todas las permutaciones
    do {
        permutaciones.push_back(sorted_username);
    } while (next_permutation(sorted_username.begin(), sorted_username.end()));
    
    debugPrint("Total de permutaciones: " + to_string(permutaciones.size()));
    
    // Encontrar la posición del username original
    int posOriginal = -1;
    for (int i = 0; i < permutaciones.size(); i++) {
        if (permutaciones[i] == username) {
            posOriginal = i;
            break;
        }
    }
    
    debugPrint("Posición del username original: " + to_string(posOriginal));
    
    // Determinar el rango de 21 permutaciones consecutivas con el username en el medio (posición 11)
    int inicio = posOriginal - 10;  // 10 antes
    int fin = posOriginal + 10;     // 10 después
    
    // Ajustar los límites si se salen del rango
    if (inicio < 0) {
        fin += abs(inicio);
        inicio = 0;
    }
    if (fin >= permutaciones.size()) {
        inicio -= (fin - permutaciones.size() + 1);
        fin = permutaciones.size() - 1;
    }
    
    // Asegurar que no salgamos de los límites después del ajuste
    inicio = max(0, inicio);
    fin = min((int)permutaciones.size() - 1, fin);
    
    debugPrint("Rango ajustado: [" + to_string(inicio) + ", " + to_string(fin) + "]");
    
    // Encontrar la permutación con la mayor distancia mínima
    int maxDistanciaMinima = -1;
    string mejorPermutacion;
    
    for (int i = inicio; i <= fin; i++) {
        int distanciaMinima = calcularDistanciaMinima(permutaciones[i]);
        debugPrint("Permutación: " + permutaciones[i] + ", Distancia mínima: " + to_string(distanciaMinima));
        
        if (distanciaMinima > maxDistanciaMinima || (distanciaMinima == maxDistanciaMinima && permutaciones[i] < mejorPermutacion)) {
            maxDistanciaMinima = distanciaMinima;
            mejorPermutacion = permutaciones[i];
        }
    }
    
    debugPrint("Mejor permutación: " + mejorPermutacion + " con distancia: " + to_string(maxDistanciaMinima));
    
    return mejorPermutacion + to_string(maxDistanciaMinima);
}

int main(int argc, char* argv[]) {
    // Verificar argumentos de línea de comandos para debug
    for(int i = 0; i < argc; i++) {
        if(string(argv[i]) == "--debug=on") {
            debug = true;
        }
    }
    
    // Modo interactivo de debug (opcional)
    if (debug) {
        bool go = true;
        while(go) {
            cout << "Debugger is now " << (debug ? "on!" : "off.") << endl;
            cout << "Turn debugger [on/off/quit/continue]: ";
            string reply;
            cin >> reply;
            if(reply == "on") debug = true;
            if(reply == "off") debug = false;
            if(reply == "quit") return 0;
            if(reply == "continue") break;
        }
    }
    
    try {
        string username;
        // Leer hasta el final del archivo
        while (getline(cin, username)) {
            if (!username.empty()) {
                debugPrint("Procesando username: " + username);
                string contrasena = generarContrasena(username);
                cout << contrasena << endl;
            }
        }
        
    } catch (const exception& e) {
        cerr << "[EXCEPCION] " << e.what() << endl;
        return 1;
    }
    
    return 0;
}