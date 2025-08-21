#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

bool debug = false;

int main(int argc, char* argv[]){
    // Activar debug con argumento de línea de comandos
    for(int i = 0; i < argc; i++)
        if(string(argv[i]) == "--debug=on")
            debug = true;
            bool go = true;
        while(go){
            if(debug){
                //Debuging code here
                cout << "Debugger is now on!" << endl;
            } else {
                cout << "Debugger is now off." << endl;
            }
                cout << "Turn debugger [on/off/quit]: ";
            string reply;
            cin >> reply;

            if(reply == "on") debug = true; //Turn it on
            if(reply == "off") debug = false; //Off
            if(reply == "quit") break; //Out of 'while'
        }

    while(true){
        if(debug){
            cout << "[DEBUG] Debug activado" << endl;
        }

        cout << "Ingrese la cantidad del cheque (o 'quit' para salir): ";
        string input;
        cin >> input;

        if(input == "quit") break;

        try {
            // Convertir input a número
            double cantidad = stod(input);

            if(cantidad < 0) throw runtime_error("Cantidad negativa no permitida");
            if(cantidad > 99999999.99) throw runtime_error("Cantidad excede los 8 espacios del cheque");

            // Convertir a string con 2 decimales
            stringstream ss;
            ss << fixed << setprecision(2) << cantidad;
            string monto = ss.str();

            // Calcular cuantos espacios faltan
            int total_espacios = 8;
            int longitud = monto.length(); // incluye punto decimal
            string protegido = "";

            if(longitud < total_espacios){
                int asteriscos = total_espacios - longitud;
                protegido = string(asteriscos, '*') + monto;
            } else {
                protegido = monto;
            }

            cout << "Cheque protegido: " << protegido << endl;

            if(debug){
                cout << "[DEBUG] Entrada: " << input << endl;
                cout << "[DEBUG] Cantidad: " << cantidad << endl;
                cout << "[DEBUG] Longitud: " << longitud << ", Asteriscos: " << (total_espacios - longitud) << endl;
                cout << "[DEBUG] Salida protegida: " << protegido << endl;
            }

        } catch(const exception& e){
            cout << "Error: " << e.what() << endl;
            if(debug){
                cout << "[DEBUG] Entrada invalida: " << input << endl;
            }
        }
    }

    return 0;
}
