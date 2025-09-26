#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <limits>

using namespace std;

bool debug = false;

void debugPrint(const string& msg) {
    if (debug) {
        cout << "[DEBUG] " << msg << endl;
    }
}

// Función para limpiar el buffer de entrada cuando hay errores
void limpiarBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Función segura para leer enteros
int leerEntero(const string& mensaje, int min = INT_MIN, int max = INT_MAX) {
    int valor;
    while (true) {
        cout << mensaje;
        
        if (cin >> valor) {
            if (valor >= min && valor <= max) {
                debugPrint("Entrada valida: " + to_string(valor));
                return valor;
            } else {
                cout << "Error: el valor debe estar entre " << min << " y " << max << endl;
            }
        } else {
            cout << "Error: eso no es un numero valido" << endl;
            debugPrint("Entrada invalida detectada, limpiando buffer");
            limpiarBuffer();
        }
    }
}

class CifradorCesar {
private:
    int clave;
    
    char procesarChar(char c, int desp) {
        if (isalpha(c)) {
            char base = islower(c) ? 'a' : 'A';
            int resultado = (c - base + desp + 26) % 26 + base;
            
            if (debug) {
                cout << "[DEBUG] '" << c << "' -> " << (char)resultado 
                     << " (desp: " << desp << ")" << endl;
            }
            
            return static_cast<char>(resultado);
        }
        return c;
    }
    
public:
    CifradorCesar(int k = 3) {
        setClave(k);
    }
    
    string cifrar(const string& texto) {
        debugPrint("Iniciando cifrado con clave " + to_string(clave));
        string resultado = texto;
        
        for (char& c : resultado) {
            c = procesarChar(c, clave);
        }
        
        debugPrint("Cifrado completado");
        return resultado;
    }
    
    string descifrar(const string& textoCifrado) {
        debugPrint("Iniciando descifrado con clave " + to_string(clave));
        string resultado = textoCifrado;
        
        for (char& c : resultado) {
            c = procesarChar(c, -clave);
        }
        
        debugPrint("Descifrado completado");
        return resultado;
    }
    
    void setClave(int nuevaClave) {
        if (nuevaClave >= 1 && nuevaClave <= 25) {
            int claveAnterior = clave;
            clave = nuevaClave;
            debugPrint("Clave cambiada de " + to_string(claveAnterior) + 
                      " a " + to_string(clave));
            cout << "Clave actualizada a: " << clave << endl;
        } else {
            cout << "Error: la clave debe estar entre 1 y 25" << endl;
            debugPrint("Intento de clave invalida: " + to_string(nuevaClave));
        }
    }
    
    int getClave() const {
        return clave;
    }
    
    void mostrarInfo() {
        cout << "\n--- INFO DEL CIFRADOR ---" << endl;
        cout << "Clave: " << clave << endl;
        cout << "Tipo: Caesar cipher" << endl;
        cout << "Solo cifra letras A-Z y a-z" << endl;
        cout << "Mantiene espacios y puntuacion" << endl;
    }
    
    void mostrarEjemplo() {
        cout << "\n--- EJEMPLO ---" << endl;
        string ejemplo = "Hola Mundo!";
        cout << "Texto original: " << ejemplo << endl;
        
        string cifrado = cifrar(ejemplo);
        cout << "Cifrado (clave " << clave << "): " << cifrado << endl;
        
        string descifrado = descifrar(cifrado);
        cout << "Descifrado: " << descifrado << endl;
        
        if (ejemplo == descifrado) {
            cout << "✓ Funciona bien" << endl;
        }
    }
};

class Menu {
private:
    CifradorCesar cifrador;
    
    void mostrarMenu() {
        cout << "\n=== CIFRADOR CESAR ===" << endl;
        cout << "1. Cifrar texto" << endl;
        cout << "2. Descifrar texto" << endl;
        cout << "3. Cambiar clave (actual: " << cifrador.getClave() << ")" << endl;
        cout << "4. Ver info" << endl;
        cout << "5. Ver ejemplo" << endl;
        cout << "6. Menu debug" << endl;
        cout << "7. Salir" << endl;
    }
    
    void cifrarTexto() {
        cout << "\n--- CIFRAR ---" << endl;
        cout << "Texto a cifrar: ";
        cin.ignore();
        string texto;
        getline(cin, texto);
        
        if (texto.empty()) {
            cout << "No escribiste nada..." << endl;
            return;
        }
        
        string resultado = cifrador.cifrar(texto);
        cout << "\nOriginal: " << texto << endl;
        cout << "Cifrado:  " << resultado << endl;
    }
    
    void descifrarTexto() {
        cout << "\n--- DESCIFRAR ---" << endl;
        cout << "Texto cifrado: ";
        cin.ignore();
        string texto;
        getline(cin, texto);
        
        if (texto.empty()) {
            cout << "No escribiste nada..." << endl;
            return;
        }
        
        string resultado = cifrador.descifrar(texto);
        cout << "\nCifrado:    " << texto << endl;
        cout << "Descifrado: " << resultado << endl;
    }
    
    void cambiarClave() {
        cout << "\n--- CAMBIAR CLAVE ---" << endl;
        cout << "Clave actual: " << cifrador.getClave() << endl;
        
        int nueva = leerEntero("Nueva clave (1-25): ", 1, 25);
        cifrador.setClave(nueva);
    }
    
    void menuDebug() {
        cout << "\n--- DEBUG MENU ---" << endl;
        cout << "Estado: " << (debug ? "ON" : "OFF") << endl;
        cout << "Comandos: on, off, test, back" << endl;
        
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
            else if (cmd == "test") {
                debugPrint("Esta es una prueba de debug");
                cout << "Test completado" << endl;
            }
            else if (cmd == "back") {
                cout << "Volviendo al menu principal..." << endl;
                break;
            }
            else {
                cout << "Comandos: on, off, test, back" << endl;
            }
        }
    }
    
public:
    Menu() : cifrador(3) {
        debugPrint("Menu inicializado con clave por defecto");
    }
    
    void ejecutar() {
        cout << "Bienvenido al cifrador Caesar!" << endl;
        
        int opcion;
        do {
            mostrarMenu();
            
            // Usar la función segura para leer enteros
            opcion = leerEntero("Opcion: ", 1, 7);
            
            switch (opcion) {
                case 1:
                    debugPrint("Usuario eligio cifrar");
                    cifrarTexto();
                    break;
                    
                case 2:
                    debugPrint("Usuario eligio descifrar");
                    descifrarTexto();
                    break;
                    
                case 3:
                    debugPrint("Usuario eligio cambiar clave");
                    cambiarClave();
                    break;
                    
                case 4:
                    debugPrint("Mostrando informacion");
                    cifrador.mostrarInfo();
                    break;
                    
                case 5:
                    debugPrint("Mostrando ejemplo");
                    cifrador.mostrarEjemplo();
                    break;
                    
                case 6:
                    debugPrint("Entrando a menu debug");
                    menuDebug();
                    break;
                    
                case 7:
                    cout << "\nHasta luego!" << endl;
                    debugPrint("Usuario salio del programa");
                    break;
            }
            
            if (opcion != 7) {
                cout << "\nPresiona Enter...";
                cin.ignore();
                cin.get();
            }
            
        } while (opcion != 7);
    }
};

int main(int argc, char* argv[]) {
    // Verificar argumentos de debug
    for (int i = 0; i < argc; i++) {
        string arg = argv[i];
        if (arg == "--debug" || arg == "-d") {
            debug = true;
            debugPrint("Debug activado desde argumentos");
        }
    }
    
    cout << "=== CIFRADOR CAESAR ===" << endl;
    
    try {
        Menu menu;
        menu.ejecutar();
    }
    catch (const exception& e) {
        cout << "Error inesperado: " << e.what() << endl;
        debugPrint("Excepcion capturada en main");
        return 1;
    }
    
    return 0;
}