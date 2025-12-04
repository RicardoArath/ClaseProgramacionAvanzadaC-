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

void limpiarBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

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

class CifradorCesarModificado {
private:
    int clave;
    static const int MIN_ASCII = 33;  // Primer caracter imprimible '!'
    static const int MAX_ASCII = 126; // Ultimo caracter imprimible '~'
    static const int RANGO = MAX_ASCII - MIN_ASCII + 1; // 94 caracteres
    
    char procesarCharCifrado(char c, int desp) {
        // Primero aplicar César solo a letras
        char resultado = c;
        
        if (isalpha(c)) {
            char base = islower(c) ? 'a' : 'A';
            resultado = (c - base + desp) % 26 + base;
            debugPrint("Cesar: '" + string(1, c) + "' -> '" + string(1, resultado) + "'");
        }
        
        // Ahora aplicar el desplazamiento de 100 de forma circular en el rango imprimible
        int valorASCII = static_cast<int>(resultado);
        
        // Convertir a posición relativa (0-93)
        int posicion = valorASCII - MIN_ASCII;
        
        // Aplicar desplazamiento circular
        int nuevaPosicion = (posicion - 100) % RANGO;
        
        // Asegurar que sea positivo
        if (nuevaPosicion < 0) {
            nuevaPosicion += RANGO;
        }
        
        // Convertir de vuelta a ASCII
        int nuevoASCII = nuevaPosicion + MIN_ASCII;
        char caracterFinal = static_cast<char>(nuevoASCII);
        
        if (debug) {
            cout << "[DEBUG] ASCII: " << valorASCII 
                 << " -> pos: " << posicion 
                 << " -> nueva pos: " << nuevaPosicion
                 << " -> ASCII final: " << nuevoASCII
                 << " ('" << caracterFinal << "')" << endl;
        }
        
        return caracterFinal;
    }
    
    char procesarCharDescifrado(char c, int desp) {
        // Revertir el desplazamiento de 100 de forma circular
        int valorASCII = static_cast<int>(c);
        
        // Convertir a posición relativa (0-93)
        int posicion = valorASCII - MIN_ASCII;
        
        // Revertir desplazamiento circular
        int nuevaPosicion = (posicion + 100) % RANGO;
        
        // Convertir de vuelta a ASCII
        int nuevoASCII = nuevaPosicion + MIN_ASCII;
        char caracterRestaurado = static_cast<char>(nuevoASCII);
        
        if (debug) {
            cout << "[DEBUG] Restaurar ASCII: " << valorASCII 
                 << " -> pos: " << posicion
                 << " -> nueva pos: " << nuevaPosicion
                 << " -> ASCII restaurado: " << nuevoASCII
                 << " ('" << caracterRestaurado << "')" << endl;
        }
        
        // Aplicar descifrado César solo a letras
        if (isalpha(caracterRestaurado)) {
            char base = islower(caracterRestaurado) ? 'a' : 'A';
            char resultado = (caracterRestaurado - base - desp + 26) % 26 + base;
            
            debugPrint("Descesar: '" + string(1, caracterRestaurado) + "' -> '" + string(1, resultado) + "'");
            return resultado;
        }
        
        return caracterRestaurado;
    }
    
public:
    CifradorCesarModificado(int k = 3) {
        setClave(k);
    }
    
    string cifrar(const string& texto) {
        debugPrint("Iniciando cifrado modificado con clave " + to_string(clave));
        string resultado = "";
        
        try {
            for (char c : texto) {
                char caracterCifrado = procesarCharCifrado(c, clave);
                resultado += caracterCifrado;
            }
            
            debugPrint("Cifrado completado exitosamente");
        }
        catch (const exception& e) {
            debugPrint("Error durante el cifrado: " + string(e.what()));
            throw;
        }
        
        return resultado;
    }
    
    string descifrar(const string& textoCifrado) {
        debugPrint("Iniciando descifrado modificado con clave " + to_string(clave));
        string resultado = "";
        
        try {
            for (char c : textoCifrado) {
                char caracterDescifrado = procesarCharDescifrado(c, clave);
                resultado += caracterDescifrado;
            }
            
            debugPrint("Descifrado completado exitosamente");
        }
        catch (const exception& e) {
            debugPrint("Error durante el descifrado: " + string(e.what()));
            throw;
        }
        
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
    
};

class Menu {
private:
    CifradorCesarModificado cifrador;
    
    void mostrarMenu() {
        cout << "\n=== CIFRADOR CESAR MODIFICADO ===" << endl;
        cout << "1. Cifrar texto" << endl;
        cout << "2. Descifrar texto" << endl;
        cout << "3. Cambiar clave (actual: " << cifrador.getClave() << ")" << endl;
        cout << "4. Menu debug" << endl;
        cout << "5. Salir" << endl;
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
        
        try {
            string resultado = cifrador.cifrar(texto);
            cout << "\nOriginal: " << texto << endl;
            cout << "Cifrado:  " << resultado << endl;
            cout << "\nValores ASCII del cifrado: ";
            for (char c : resultado) {
                cout << "[" << static_cast<int>(c) << "]";
            }
            cout << endl;
        }
        catch (const exception& e) {
            cout << "Error al cifrar: " << e.what() << endl;
            debugPrint("Excepcion en cifrarTexto");
        }
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
        
        try {
            string resultado = cifrador.descifrar(texto);
            cout << "\nCifrado:    " << texto << endl;
            cout << "Descifrado: " << resultado << endl;
        }
        catch (const exception& e) {
            cout << "Error al descifrar: " << e.what() << endl;
            debugPrint("Excepcion en descifrarTexto");
        }
    }
    
    void cambiarClave() {
        cout << "\n--- CAMBIAR CLAVE ---" << endl;
        cout << "Clave actual: " << cifrador.getClave() << endl;
        
        try {
            int nueva = leerEntero("Nueva clave (1-25): ", 1, 25);
            cifrador.setClave(nueva);
        }
        catch (const exception& e) {
            cout << "Error al cambiar clave: " << e.what() << endl;
            debugPrint("Excepcion en cambiarClave");
        }
    }
    
    void menuDebug() {
        cout << "\n--- DEBUG MENU ---" << endl;
        cout << "Estado: " << (debug ? "ON" : "OFF") << endl;
        cout << "Comandos: on, off, test, back" << endl;
        
        string cmd;
        while (true) {
            cout << "debug> ";
            cin >> cmd;
            
            try {
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
            catch (const exception& e) {
                cout << "Error en menu debug: " << e.what() << endl;
                debugPrint("Excepcion en menuDebug");
            }
        }
    }
    
public:
    Menu() : cifrador(3) {
        debugPrint("Menu inicializado con clave por defecto");
    }
    
    void ejecutar() {
        cout << "Bienvenido al cifrador Caesar modificado!" << endl;
        cout << "Trabaja con caracteres ASCII imprimibles (33-126)" << endl;
        
        int opcion;
        do {
            try {
                mostrarMenu();
                opcion = leerEntero("Opcion: ", 1, 5);
                
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
                        debugPrint("Entrando a menu debug");
                        menuDebug();
                        break;
                        
                    case 5:
                        cout << "\nHasta luego!" << endl;
                        debugPrint("Usuario salio del programa");
                        break;
                }
                
                if (opcion != 5) {
                    cout << "\nPresiona Enter...";
                    cin.ignore();
                    cin.get();
                }
            }
            catch (const exception& e) {
                cout << "Error inesperado en el menu: " << e.what() << endl;
                debugPrint("Excepcion capturada en ejecutar");
                opcion = 0;
            }
            
        } while (opcion != 5);
    }
};

int main(int argc, char* argv[]) {
    for (int i = 0; i < argc; i++) {
        string arg = argv[i];
        if (arg == "--debug" || arg == "-d") {
            debug = true;
            debugPrint("Debug activado desde argumentos");
        }
    }
    
    cout << "=== CIFRADOR CAESAR MODIFICADO ===" << endl;
    
    try {
        Menu menu;
        menu.ejecutar();
    }
    catch (const exception& e) {
        cout << "Error critico: " << e.what() << endl;
        debugPrint("Excepcion critica capturada en main");
        return 1;
    }
    
    return 0;
}