/*
Ejercicio 2.- En la correspondencia de negocios, las fechas se imprimen comúnmente en varios formatos diferentes. Dos de los formatos más comunes son:

0 7 / 2 1 / 2025  

Julio  2 1 , 2025

21 de Julio de 2025

Escriba un programa que lea una fecha en el primer formato y la imprima en el segundo y tercero

*/

#include <iostream>
#include <sstream>
#include <string>
#include <map>
using namespace std;

bool debug = false; // bandera de debug

int main(int argc, char* argv[]) {
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
    
     // Map de meses
    map<int, string> meses = {
        {1,"Enero"}, {2,"Febrero"}, {3,"Marzo"}, {4,"Abril"}, {5,"Mayo"}, {6,"Junio"},
        {7,"Julio"}, {8,"Agosto"}, {9,"Septiembre"}, {10,"Octubre"}, {11,"Noviembre"}, {12,"Diciembre"}
    };

    while(true){
        if(debug){
            cout << "[DEBUG] Debug activado" << endl;
        }

        cout << "Ingrese la fecha en formato MM/DD/YYYY (o 'quit' para salir): ";
        string input;
        cin >> input;

        if(input == "quit") break;

        try {
            stringstream ss(input);
            string mes_str, dia_str, anio_str;

            // Separar por '/'
            if(!getline(ss, mes_str, '/')) throw runtime_error("Formato incorrecto (mes)");
            if(!getline(ss, dia_str, '/')) throw runtime_error("Formato incorrecto (día)");
            if(!getline(ss, anio_str, '/')) throw runtime_error("Formato incorrecto (año)");

            int mes = stoi(mes_str);
            int dia = stoi(dia_str);
            int anio = stoi(anio_str);

            if(mes < 1 || mes > 12) throw runtime_error("Mes inválido");
            if(dia < 1 || dia > 31) throw runtime_error("Día inválido");

            // Formato: Julio 21, 2025
            cout << meses[mes] << " " << dia << ", " << anio << endl;
            // Formato: 21 de Julio de 2025
            cout << dia << " de " << meses[mes] << " de " << anio << endl;

            if(debug){
                cout << "[DEBUG] Mes: " << mes << ", Dia: " << dia << ", Anio: " << anio << endl;
            }

        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
            if(debug){
                cout << "[DEBUG] Entrada recibida: " << input << endl;
            }
        }
    }

    return 0;
}
    