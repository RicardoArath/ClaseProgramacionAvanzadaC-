#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>
#include <vector>

using namespace std;

bool debug = false;

void debugPrint(const string& mensaje) {
    if (debug) {
        cout << "[DEBUG] " << mensaje << endl;
    }
}

string limpiarClave(const string& clave) {
    debugPrint("Limpiando clave: '" + clave + "'");
    string resultado = "";
    for (char c : clave) {
        if (isalpha(c)) {
            resultado += toupper(c);
        }
    }
    debugPrint("Clave limpia: '" + resultado + "'");
    return resultado;
}

string repetirClave(const string& texto, const string& clave) {
    if (clave.empty()) return "";
    
    debugPrint("Expandiendo clave para texto de " + to_string(texto.length()) + " caracteres");
    string claveExpandida = "";
    int pos = 0;
    
    for (int i = 0; i < texto.length(); i++) {
        if (isalpha(texto[i])) {
            claveExpandida += clave[pos % clave.length()];
            pos++;
        } else {
            claveExpandida += ' ';
        }
    }
    
    if (debug) {
        cout << "[DEBUG] Clave expandida: ";
        for (int i = 0; i < claveExpandida.length(); i++) {
            if (isalpha(texto[i])) cout << claveExpandida[i];
            else cout << "_";
        }
        cout << endl;
    }
    
    return claveExpandida;
}

string cifrar(const string& texto, const string& clave) {
    debugPrint("=== INICIANDO CIFRADO ===");
    
    string claveList = limpiarClave(clave);
    if (claveList.empty()) {
        cout << "Error: necesitas al menos una letra en la clave" << endl;
        return texto;
    }
    
    string claveExp = repetirClave(texto, claveList);
    string resultado = "";
    
    debugPrint("Procesando caracter por caracter...");
    
    for (int i = 0; i < texto.length(); i++) {
        char c = texto[i];
        
        if (isalpha(c)) {
            bool mayus = isupper(c);
            int numTexto = toupper(c) - 'A';
            int numClave = claveExp[i] - 'A';
            int numFinal = (numTexto + numClave) % 26;
            
            char letraFinal = numFinal + (mayus ? 'A' : 'a');
            resultado += letraFinal;
            
            if (debug) {
                cout << "[DEBUG] '" << c << "' + '" << claveExp[i] << "' -> "
                     << numTexto << " + " << numClave << " = " << numFinal 
                     << " -> '" << letraFinal << "'" << endl;
            }
        } else {
            resultado += c;
            debugPrint("Manteniendo: '" + string(1, c) + "'");
        }
    }
    
    debugPrint("=== CIFRADO COMPLETADO ===");
    return resultado;
}

string descifrar(const string& textoCifrado, const string& clave) {
    debugPrint("=== INICIANDO DESCIFRADO ===");
    
    string claveList = limpiarClave(clave);
    if (claveList.empty()) {
        cout << "Error: necesitas al menos una letra en la clave" << endl;
        return textoCifrado;
    }
    
    string claveExp = repetirClave(textoCifrado, claveList);
    string resultado = "";
    
    for (int i = 0; i < textoCifrado.length(); i++) {
        char c = textoCifrado[i];
        
        if (isalpha(c)) {
            bool mayus = isupper(c);
            int numCifrado = toupper(c) - 'A';
            int numClave = claveExp[i] - 'A';
            int numOriginal = (numCifrado - numClave + 26) % 26;
            
            char letraOriginal = numOriginal + (mayus ? 'A' : 'a');
            resultado += letraOriginal;
            
            if (debug) {
                cout << "[DEBUG] '" << c << "' - '" << claveExp[i] << "' -> "
                     << numCifrado << " - " << numClave << " = " << numOriginal 
                     << " -> '" << letraOriginal << "'" << endl;
            }
        } else {
            resultado += c;
        }
    }
    
    debugPrint("=== DESCIFRADO COMPLETADO ===");
    return resultado;
}

void mostrarTabla(const string& texto, const string& clave) {
    string claveList = limpiarClave(clave);
    string claveExp = repetirClave(texto, claveList);
    
    cout << "\nPaso a paso:" << endl;
    cout << "Texto:  ";
    for (int i = 0; i < texto.length(); i++) {
        if (isalpha(texto[i])) cout << texto[i] << " ";
        else cout << "  ";
    }
    cout << endl;
    
    cout << "Clave:  ";
    for (int i = 0; i < texto.length(); i++) {
        if (isalpha(texto[i])) cout << claveExp[i] << " ";
        else cout << "  ";
    }
    cout << endl;
    
    cout << "Nums:   ";
    for (int i = 0; i < texto.length(); i++) {
        if (isalpha(texto[i])) {
            int n = claveExp[i] - 'A';
            if (n < 10) cout << n << " ";
            else cout << n << "";
        } else {
            cout << "  ";
        }
    }
    cout << endl;
}

void checkClave(const string& clave) {
    string claveList = limpiarClave(clave);
    
    cout << "\nInfo de la clave:" << endl;
    cout << "Original: " << clave << endl;
    cout << "Procesada: " << claveList << endl;
    cout << "Tamaño: " << claveList.length() << " letras" << endl;
    
    if (claveList.length() < 4) {
        cout << "-> Clave corta (debil)" << endl;
    } else if (claveList.length() < 8) {
        cout << "-> Clave normal" << endl;
    } else {
        cout << "-> Clave larga (fuerte)" << endl;
    }
    
    // Ver si tiene letras repetidas seguidas
    bool repetidas = false;
    for (int i = 0; i < claveList.length() - 1; i++) {
        if (claveList[i] == claveList[i + 1]) {
            repetidas = true;
            break;
        }
    }
    
    if (repetidas) {
        cout << "-> Tiene letras seguidas iguales" << endl;
    }
}

void menuDebug() {
    cout << "\n--- Control de Debug ---" << endl;
    cout << "Estado actual: " << (debug ? "ACTIVADO" : "DESACTIVADO") << endl;
    cout << "Comandos: on, off, status, help, quit" << endl;
    
    string cmd;
    while (true) {
        cout << "debug> ";
        cin >> cmd;
        
        if (cmd == "on") {
            debug = true;
            cout << "Debug activado" << endl;
        }
        else if (cmd == "off") {
            debug = false;
            cout << "Debug desactivado" << endl;
        }
        else if (cmd == "status") {
            cout << "Debug esta " << (debug ? "activado" : "desactivado") << endl;
        }
        else if (cmd == "help") {
            cout << "Comandos disponibles:" << endl;
            cout << "  on     - activar debug" << endl;
            cout << "  off    - desactivar debug" << endl;
            cout << "  status - ver estado actual" << endl;
            cout << "  quit   - salir del menu debug" << endl;
        }
        else if (cmd == "quit") {
            break;
        }
        else {
            cout << "Comando no reconocido. Usa 'help' para ver opciones." << endl;
        }
    }
}

void ejemplo() {
    cout << "\n=== EJEMPLO ===" << endl;
    cout << "Vamos a cifrar: 'HOLA MUNDO'" << endl;
    cout << "Con la clave: 'CLAVE'" << endl;
    
    string txt = "HOLA MUNDO";
    string key = "CLAVE";
    
    mostrarTabla(txt, key);
    
    string cifrado = cifrar(txt, key);
    cout << "\nCifrado: " << cifrado << endl;
    
    string descifrado = descifrar(cifrado, key);
    cout << "Descifrado: " << descifrado << endl;
    
    if (txt == descifrado) {
        cout << "✓ Funciona correctamente" << endl;
    } else {
        cout << "✗ Algo salio mal" << endl;
    }
}

int main(int argc, char* argv[]) {
    // Revisar argumentos
    for (int i = 0; i < argc; i++) {
        if (string(argv[i]) == "--debug" || string(argv[i]) == "-d") {
            debug = true;
            debugPrint("Debug activado desde argumentos");
        }
    }

    cout << "=== CIFRADOR VIGENERE ===" << endl;
    cout << "Escrito en C++ con funciones de debug" << endl;
    
    string texto, clave;
    int opcion;
    
    do {
        cout << "\n1. Cifrar" << endl;
        cout << "2. Descifrar" << endl;
        cout << "3. Ver ejemplo" << endl;
        cout << "4. Menu debug" << endl;
        cout << "5. Salir" << endl;
        cout << "Opcion: ";
        cin >> opcion;
        
        switch (opcion) {
            case 1: {
                cout << "\nTexto a cifrar: ";
                cin.ignore();
                getline(cin, texto);
                cout << "Clave: ";
                getline(cin, clave);
                
                checkClave(clave);
                mostrarTabla(texto, clave);
                
                string resultado = cifrar(texto, clave);
                
                cout << "\n--- RESULTADO ---" << endl;
                cout << "Original: " << texto << endl;
                cout << "Clave: " << limpiarClave(clave) << endl;
                cout << "Cifrado: " << resultado << endl;
                break;
            }
            
            case 2: {
                cout << "\nTexto cifrado: ";
                cin.ignore();
                getline(cin, texto);
                cout << "Clave: ";
                getline(cin, clave);
                
                string resultado = descifrar(texto, clave);
                
                cout << "\n--- RESULTADO ---" << endl;
                cout << "Cifrado: " << texto << endl;
                cout << "Clave: " << limpiarClave(clave) << endl;
                cout << "Descifrado: " << resultado << endl;
                break;
            }
            
            case 3:
                ejemplo();
                break;
                
            case 4:
                menuDebug();
                break;
                
            case 5:
                cout << "\nBye!" << endl;
                break;
                
            default:
                cout << "Opcion no valida" << endl;
                break;
        }
        
        if (opcion != 5 && opcion >= 1 && opcion <= 4) {
            cout << "\nPresiona Enter...";
            cin.ignore();
            cin.get();
        }
        
    } while (opcion != 5);

    return 0;
}