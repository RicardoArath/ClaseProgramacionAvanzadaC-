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

// Función para encontrar la subsecuencia común más larga
string encontrarSubsecuenciaComun(const string& cadena1, const string& cadena2) {
    int largo1 = cadena1.length();
    int largo2 = cadena2.length();
    
    debugPrint("cadena1 = " + cadena1 + " (longitud: " + to_string(largo1) + ")");
    debugPrint("cadena2 = " + cadena2 + " (longitud: " + to_string(largo2) + ")");
    
    // Matriz para almacenar las longitudes de subsecuencias
    vector<vector<int>> matrizDP(largo1 + 1, vector<int>(largo2 + 1, 0));
    
    // Llenar la matriz usando programación dinámica
    for (int fila = 1; fila <= largo1; fila++) {
        for (int columna = 1; columna <= largo2; columna++) {
            if (cadena1[fila - 1] == cadena2[columna - 1]) {
                // Los caracteres coinciden, incrementar desde diagonal
                matrizDP[fila][columna] = matrizDP[fila - 1][columna - 1] + 1;
            } else {
                // No coinciden, tomar el máximo de arriba o izquierda
                matrizDP[fila][columna] = max(matrizDP[fila - 1][columna], 
                                              matrizDP[fila][columna - 1]);
            }
        }
    }
    
    int longitudFinal = matrizDP[largo1][largo2];
    debugPrint("Longitud de subsecuencia común: " + to_string(longitudFinal));
    
    // Mostrar la matriz si debug está activo
    if (debug) {
        cout << "[DEBUG] Matriz de programación dinámica:" << endl;
        cout << "      ";
        for (int col = 0; col < largo2; col++) {
            cout << cadena2[col] << " ";
        }
        cout << endl;
        for (int fil = 0; fil <= largo1; fil++) {
            if (fil > 0) cout << cadena1[fil-1] << " ";
            else cout << "  ";
            for (int col = 0; col <= largo2; col++) {
                cout << matrizDP[fil][col] << " ";
            }
            cout << endl;
        }
    }
    
    // Reconstruir la secuencia desde el final
    string subsecuencia = "";
    int posActual1 = largo1;
    int posActual2 = largo2;
    
    // Retroceder desde la esquina inferior derecha
    while (posActual1 > 0 && posActual2 > 0) {
        char caracterActual1 = cadena1[posActual1 - 1];
        char caracterActual2 = cadena2[posActual2 - 1];
        
        if (caracterActual1 == caracterActual2) {
            // Carácter común encontrado, agregarlo al inicio
            subsecuencia = caracterActual1 + subsecuencia;
            debugPrint("Carácter común: " + string(1, caracterActual1));
            posActual1--;
            posActual2--;
        } else {
            // Moverse hacia donde está el valor mayor
            int valorArriba = matrizDP[posActual1 - 1][posActual2];
            int valorIzquierda = matrizDP[posActual1][posActual2 - 1];
            
            if (valorArriba > valorIzquierda) {
                posActual1--;
            } else {
                posActual2--;
            }
        }
    }
    
    debugPrint("Subsecuencia final: " + subsecuencia);
    
    return subsecuencia;
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
        string primeraLinea, segundaLinea;
        
        // Leer pares de cadenas hasta EOF
        while (getline(cin, primeraLinea)) {
            if (getline(cin, segundaLinea)) {
                debugPrint("========== Procesando nuevo par ==========");
                string resultado = encontrarSubsecuenciaComun(primeraLinea, segundaLinea);
                cout << resultado << endl;
            }
        }
        
    } catch (const exception& e) {
        cerr << "[EXCEPCION] " << e.what() << endl;
        return 1;
    }
    
    return 0;
}