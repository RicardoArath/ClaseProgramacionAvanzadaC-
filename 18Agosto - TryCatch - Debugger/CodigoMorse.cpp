#include "iostream"
#include <string>
#include <limits>
#include <sstream>
using namespace std;
//Codigo morse. convertir palabras a codigo morse y viceversa.

bool debug = false;
string frase;
string morse[] = {".-", "-...", "-.-.","-..",".","..-.","--.","....","..",".---","-.-",".-..","--","-.","---",".--.","--.-",".-.","...","-","..-","...-",".--","-..-","-.--","--.."};
string abecedario[] = {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"};

string convertirTextoAMorse(){
    while(true){
        cout << "Dame una palabra o un texto: (o 'quit' para salir)" << endl;
        getline(cin,frase);
        if (frase.empty()) throw runtime_error("ERROR: No se ingreso ningun texto.");
        
        if (debug) cout << "[DEBUG] Frase leida: " << frase << endl;

        if(frase == "quit") break;
        
        try {
            //PALABRA A CODIGO MORSE
            string palabraMorse;
            for(int i = 0; i < frase.length();i++){
                frase[i] = tolower(frase[i]);   //Pasamos cada caracter a minuscula para poder buscarlo mas facil
                if(frase[i] == 'ñ'){
                    if (debug) cout << "[DEBUG] Se encontro la letra: " << "ñ" <<" que va con el codigo morse: --.--"<< endl;
                    palabraMorse += "--.-- "; // Morse real de la ñ
                    continue;
                }
                if(frase[i] == ' '){
                    if (debug) cout << "[DEBUG] Se encontro un espacio " << endl;
                    palabraMorse+=" ";    //cuando hay un espacio se coloca en el texto
                } else {
                    for(int j = 0; j < 26; j++){
                        if(frase[i] == abecedario[j][0]){
                            if (debug) cout << "[DEBUG] Se encontro la letra: " << abecedario[j][0] <<" que va con el codigo morse: "<<morse[j] << endl;
                            palabraMorse+=morse[j] + " ";
                            break;
                        }
                    }   
                }
            }
            if (debug) cout << "[DEBUG] Longitud de la palabra: " << frase.length() << endl;
            cout << "La palabra en codigo Morse es: " << palabraMorse << endl;
        } catch (const exception& e) {
        cerr << "[EXCEPCION] " << e.what() << endl;
        }
    }
}

void convertirMorseATexto(){
    while(true){
        cout << "Dame una palabra o un texto en codigo morse: (o 'quit' para salir)" << endl;
        getline(cin,frase);
        if (frase.empty()) throw runtime_error("ERROR: No se ingreso ningun texto.");
        
        if (debug) cout << "[DEBUG] Codigo morse leido: " << frase << endl;

        if(frase == "quit") break;
        
        try {
            string palabraNormal;
            stringstream ss(frase);
            string simbolo;

            // leemos cada grupo de puntos/rayas separado por espacio
            while(ss >> simbolo){
                bool encontrado = false;

                // caso especial: ñ
                if(simbolo == "--.--"){
                    palabraNormal += "ñ";
                    encontrado = true;
                }

                // buscar en el arreglo de morse
                for(int j = 0; j < 26 && !encontrado; j++){
                    if(simbolo == morse[j]){
                        palabraNormal += abecedario[j];
                        encontrado = true;
                    }
                }

                // si no se encontró nada
                if(!encontrado){
                    if(debug) cout << "[DEBUG] Simbolo desconocido: " << simbolo << endl;
                }
            }

            cout << "El texto traducido es: " << palabraNormal << endl;
        } catch (const exception& e) {
            cerr << "[EXCEPCION] " << e.what() << endl;
        }
    }
}


int main(int argc, char* argv[]){
    // Activar debug con argumento de línea de comandos
    for(int i = 0; i < argc; i++)
        if(string(argv[i]) == "--debug=on"){
            debug = true;
        }
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

    try {
        int op;
        if(debug) cout << "[DEBUG] Debug activado" << endl;

        cout << "[1] Convertir palabra a codigo morse " <<endl;
        cout << "[2] Convertir codigo morse a palabra" <<endl;
        cout << "Seleccione una opcion tecleando el numero de la opcion deseada: " <<endl;
        cin >> op;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        while(op == 1 || op == 2){
            if(debug) cout <<"[DEBUG] El usuario ha seleccionado la opcion " << op << endl;
            switch(op){
            case 1: 
                convertirTextoAMorse();
                break;

            case 2: 
                convertirMorseATexto();
                break;
            }
        }
    } catch (const exception& e) {
        cerr << "[EXCEPCION] " << e.what() << endl;
        
    }

    return 0;
}