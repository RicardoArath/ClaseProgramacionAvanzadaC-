#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

bool debug = false;

void debugPrint(const string& mensaje) {
    if (debug) {
        cout << "[DEBUG] " << mensaje << endl;
    }
}

string descifradoSRC(string palabra){
    
    // Paso 1: Dividir
    int mitad = palabra.length() / 2;
    string primera_mitad = palabra.substr(0, mitad);
    debugPrint("Primera mitad: " + primera_mitad);
    string segunda_mitad = palabra.substr(mitad);
    debugPrint("Segunda mitad: " + segunda_mitad);

    // Función para calcular rotación
    auto calcularRotacion = [](const string& cadena) -> int {
        int suma = 0;
        for (char c : cadena) {
            suma += (c - 'A');
            debugPrint("Sumando valor de '" + string(1,c) + "': " + to_string(c - 'A'));
        }
        return suma;
    };

    // Función para rotar una cadena
    auto rotarCadena = [](const string& cadena, int rotacion) -> string {
        string resultado = "";
        for (char c : cadena) {
            int valor = c - 'A';
            int nuevo_valor = (valor + rotacion) % 26;
            resultado += char(nuevo_valor + 'A');
            debugPrint("Rotando '" + string(1,c) + "' por " + to_string(rotacion) + 
                       ": " + string(1,char(nuevo_valor + 'A')));
        }
        return resultado;
    };

    // Paso 2: Rotar cada mitad
    int rotacion_primera_mitad = calcularRotacion(primera_mitad);
    int rotacion_segunda_mitad = calcularRotacion(segunda_mitad);
    
    string primera_mitad_rotada = rotarCadena(primera_mitad, rotacion_primera_mitad);
    string segunda_mitad_rotada = rotarCadena(segunda_mitad, rotacion_segunda_mitad);
    debugPrint("Primera mitad rotada: " + primera_mitad_rotada);
    debugPrint("Segunda mitad rotada: " + segunda_mitad_rotada);

    // Paso 3: Combinar
    string mensaje_descifrado = "";
    for (int i = 0; i < primera_mitad_rotada.length(); i++) {
        char char_primera = primera_mitad_rotada[i];
        char char_segunda = segunda_mitad_rotada[i];
        
        // Valor del carácter de la segunda mitad
        int valor_segunda = char_segunda - 'A';
        
        // Rotar el carácter de la primera mitad por este valor
        int valor_primera = char_primera - 'A';
        int nuevo_valor = (valor_primera + valor_segunda) % 26;
        
        mensaje_descifrado += char(nuevo_valor + 'A');
    }
    
    return mensaje_descifrado;
    
}

int main(int argc, char* argv[]) {
    // Revisar argumentos
    for (int i = 0; i < argc; i++) {
        if (string(argv[i]) == "--debug" || string(argv[i]) == "-d") {
            debug = true;
            debugPrint("Debug activado desde argumentos");
        }
    }

    try {
        cout << "=== DESCIFRADOR SRC ===" << endl;
        cout << "Ingrese el numero de cifrados a realizar, seguido de las palabras a cifrar: "<<endl;
        int k;
        cin >> k;
        cin.ignore();

    for(int i = 0; i < k; i++){
        string input;
        getline(cin,input);
        debugPrint("Recibiendo input" + input);
        cout << descifradoSRC(input)<<endl;
    }

    } catch (const exception& e) {
        cout << "Error inesperado: " << e.what() << endl;
        debugPrint("Excepcion capturada en main");
        return 1;
    }

    return 0;
}