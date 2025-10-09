#include <iostream>
#include <regex>
#include <string>

using namespace std;

bool debug = false;

int main(int argc, char* argv[]) {
    // Activar debug con argumento de línea de comandos
    for (int i = 0; i < argc; i++)
        if (string(argv[i]) == "--debug=on")
            debug = true;

    // Menú interactivo de debugger
    bool go = true;
    while (go) {
        if (debug) {
            cout << "[DEBUG] Debug activado" << endl;
        } else {
            cout << "[DEBUG] Debug desactivado" << endl;
        }

        cout << "Turn debugger [on/off/quit]: ";
        string reply;
        cin >> reply;
        cin.ignore(); // limpiar buffer

        if (reply == "on") debug = true;
        if (reply == "off") debug = false;
        if (reply == "quit") break;
    }

    while (true) {
        cout << "Ingrese el texto (o 'quit' para salir): ";
        string response;
        getline(cin, response);

        if (response == "quit") break;

        try {
            // Expresiones regulares
            regex email_pattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
            regex enterprise_pattern(R"("([^"]+?)\s*(?:S\.A\.|S\.R\.L\.|LLC)?")");
            regex zipCode_pattern(R"(CP\s*(\d{4,5}))");

            // Extraer código postal
            smatch coincidences_zipCode;
            if (regex_search(response, coincidences_zipCode, zipCode_pattern)) {
                cout << "Codigo postal encontrado: " << coincidences_zipCode[1] << endl;
            } else {
                cout << "No se encontro codigo postal." << endl;
            }

            // Extraer correo electrónico
            smatch coincidences_email;
            if (regex_search(response, coincidences_email, email_pattern)) {
                cout << "Correo encontrado: " << coincidences_email[0] << endl;
            } else {
                cout << "No se encontro correo electronico." << endl;
            }

            // Extraer nombre de empresa
            smatch coincidences_enterprise;
            if (regex_search(response, coincidences_enterprise, enterprise_pattern)) {
                cout << "Nombre de empresa encontrado: " << coincidences_enterprise[1] << endl;
            } else {
                cout << "No se encontro nombre de empresa." << endl;
            }

            if (debug) {
                cout << "[DEBUG] Texto ingresado: " << response << endl;
                cout << "[DEBUG] Coincidencias - CP: " << (coincidences_zipCode.empty() ? "N/A" : coincidences_zipCode[1].str())
                     << ", Email: " << (coincidences_email.empty() ? "N/A" : coincidences_email[0].str())
                     << ", Empresa: " << (coincidences_enterprise.empty() ? "N/A" : coincidences_enterprise[1].str())
                     << endl;
            }

        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
            if (debug) {
                cout << "[DEBUG] Excepcion atrapada mientras se procesaba el texto." << endl;
            }
        }
    }

    return 0;
}
