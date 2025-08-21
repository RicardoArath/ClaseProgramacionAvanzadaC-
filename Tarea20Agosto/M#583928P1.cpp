/*Ejercicio 1.-  El tratamiento detallado de la manipulación de cadenas en este texto debe atribuirse principalmente al excitante crecimiento, en años recientes, del procesamiento de texto. Una función importante de los sistemas de procesamiento de palabras es el tipo justificado de las palabras, tanto en los márgenes izquierdo como derecho de una página. Esto genera un documento de apariencia profesional, que parece haber sido formado en tipografía, en vez de preparado en una máquina de escribir. El tipo justificado puede ser llevado a cabo en sistemas de computación, insertando uno o más caracteres en blanco entre cada una de las palabras de una línea, de tal forma que la palabra más a la derecha se alinee con el margen derecho.

Escriba un programa que lea varias líneas de texto e imprima este texto en formato de tipo justificado. Suponga que el texto debe ser impreso en papel de un ancho de 8 1/2 pulgadas, y que se deben dejarmárgenes de una pulgada, tanto en el lado derecho como izquierdo de la página impresa. Suponga que la computadora imprime 10 caracteres por cada pulgada horizontal.

Por lo tanto, su programa deberá imprimir 6 1/2 pulgadas de texto, es decir, 65 caracteres por línea. 8.36 (Im presión de fech a s en varios form atos). */


#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>
using namespace std;

const int LINE_WIDTH = 65;
bool debug = false; // bandera de debug

// Función para justificar una línea de palabras
string justificarTexto(const vector<string>& palabras, int width, bool isLastLine) {
    if (palabras.empty()) return "";

    if (palabras.size() == 1 || isLastLine) {
        string line = palabras[0];
        for (int i = 1; i < palabras.size(); i++) {
            line += " " + palabras[i];
        }
        if ((int)line.size() < width)
            line.append(width - line.size(), ' ');
        return line;
    }

    int totalChars = 0;
    for (auto &w : palabras) totalChars += w.size();

    int totalSpaces = width - totalChars;
    int gaps = palabras.size() - 1;
    int spacePerGap = totalSpaces / gaps;
    int extraSpaces = totalSpaces % gaps;

    string line;
    for (int i = 0; i < (int)palabras.size(); i++) {
        line += palabras[i];
        if (i < gaps) {
            line.append(spacePerGap + (i < extraSpaces ? 1 : 0), ' ');
        }
    }
    return line;
}

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

    try {
        string line;
        vector<string> palabras;

        cout << "Escriba el texto (termina con Ctrl+Z/Ctrl+D + Enter):\n";
        while (true) {
            if (!getline(cin, line)) break;   // rompe con EOF (Ctrl+D/Ctrl+Z)
            stringstream ss(line);
            string word;
            while (ss >> word) {
                palabras.push_back(word);
            }
        }

        if (palabras.empty()) {
            throw runtime_error("ERROR: No se ingreso ningun texto.");
        }

        if (debug) cout << "[DEBUG] Palabras leidas: " << palabras.size() << endl;

        vector<string> linepalabras;
        int currentLength = 0;

        for (int i = 0; i < (int)palabras.size(); i++) {
            string w = palabras[i];
            if (debug) cout << "[DEBUG] Procesando palabra: " << w << endl;

            // linepalabras.size() representa los espacios mínimos entre palabras
            if (currentLength + (int)w.size() + (int)linepalabras.size() <= LINE_WIDTH) {
                linepalabras.push_back(w);
                currentLength += w.size();
                if (debug) cout << "[DEBUG] Añadida a la linea. Longitud actual: " << currentLength << endl;
            } else {
                cout << justificarTexto(linepalabras, LINE_WIDTH, false) << "\n";
                if (debug) cout << "[DEBUG] Linea justificada e impresa." << endl;

                linepalabras.clear();
                linepalabras.push_back(w);
                currentLength = w.size();
            }
        }

        if (!linepalabras.empty()) {
            cout << justificarTexto(linepalabras, LINE_WIDTH, true) << "\n";
            if (debug) cout << "[DEBUG] Última línea impresa (alineada izquierda)." << endl;
        }

        cin.clear(); // resetea flags (eofbit/failbit)


    } catch (const exception& e) {
        cerr << "[EXCEPCIÓN] " << e.what() << endl;
        return 1;
    }

    return 0;
}
