// Programa realizado por:
// Alumno 1: Nombre1 Matricula1
// Alumno 2: Nombre2 Matricula2

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include <windows.h> // Para SetConsoleOutputCP

using namespace std;

// Estructuras de datos
struct Contacto {
    string email;
    string telefono;
};

struct Empleo {
    string puesto;
    string empresa;
};

struct Persona {
    int id = 0;
    string nombre;
    int edad = 0;
    string ciudad;
    Contacto contacto;
    Empleo empleo;
};

// Funciones de validación
bool validarNombre(const string& s) {
    regex r("^[A-Za-zÁÉÍÓÚáéíóúÑñ ]+$");
    return regex_match(s, r);
}

bool validarEdad(const string& s) {
    regex r("^\\d+$");
    return regex_match(s, r);
}

bool validarCiudad(const string& s) {
    regex r("^[A-Za-zÁÉÍÓÚáéíóúÑñ ]+$");
    return regex_match(s, r);
}

bool validarEmail(const string& s) {
    regex r("^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}$");
    return regex_match(s, r);
}

bool validarTelefono(const string& s) {
    regex r("^\\d{3}-\\d{3}-\\d{4}$");
    return regex_match(s, r);
}

bool validarPuesto(const string& s) {
    regex r("^[A-Za-zÁÉÍÓÚáéíóúÑñ\\s?]+$");
    return regex_match(s, r);
}

bool validarEmpresa(const string& s) {
    regex r("^[A-Za-zÁÉÍÓÚáéíóúÑñ.\\s?]+$.?");
    return regex_match(s, r);
}

// Función para extraer valor de una línea
string extraerValor(const string& linea, const string& clave, bool esNumero = false) {
    smatch match;
    if (esNumero) {
        regex r("\"" + clave + "\"\\s*:\\s*(\\d+)");
        if (regex_search(linea, match, r)) return match[1];
    } else {
        regex r("\"" + clave + "\"\\s*:\\s*\"([^\"]+)\"");
        if (regex_search(linea, match, r)) return match[1];
    }
    return "";
}

int main() {
    // Configurar consola para UTF-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    ifstream archivoLeer("personas.json");
    if (!archivoLeer.is_open()) {
        cerr << "Error al abrir el archivo." << endl;
        return 1;
    }

    string linea;
    vector<Persona> personas;
    Persona p;

    while (getline(archivoLeer, linea)) {
        string valor;

        // Leer id
        valor = extraerValor(linea, "id", true);
        if (!valor.empty()) p.id = stoi(valor);

        // Leer nombre
        valor = extraerValor(linea, "nombre");
        if (!valor.empty() && validarNombre(valor)) p.nombre = valor;

        // Leer edad
        valor = extraerValor(linea, "edad", true);
        if (!valor.empty() && validarEdad(valor)) p.edad = stoi(valor);

        // Leer ciudad
        valor = extraerValor(linea, "ciudad");
        if (!valor.empty() && validarCiudad(valor)) p.ciudad = valor;

        // Leer email
        valor = extraerValor(linea, "email");
        if (!valor.empty() && validarEmail(valor)) p.contacto.email = valor;

        // Leer telefono
        valor = extraerValor(linea, "telefono");
        if (!valor.empty() && validarTelefono(valor)) p.contacto.telefono = valor;

        // Leer puesto
        valor = extraerValor(linea, "puesto");
        if (!valor.empty() && validarPuesto(valor)) p.empleo.puesto = valor;

        // Leer empresa
        valor = extraerValor(linea, "empresa");
        if (!valor.empty() && validarEmpresa(valor)){
            p.empleo.empresa = valor;
            personas.push_back(p);
            p = Persona();
        }

        // Cada persona termina cuando encontramos el cierre del objeto }
       /* if (regex_search(linea, regex("^\\s*}\\s*(,)?\\s*$")) && p.id != 0) {
            personas.push_back(p);
            p = Persona();
        }
       */     
    }

    archivoLeer.close();

    // Imprimir listado
    for (auto& persona : personas) {
        cout << "ID: " << persona.id << endl;
        cout << "Nombre: " << persona.nombre << endl;
        cout << "Edad: " << persona.edad << endl;
        cout << "Ciudad: " << persona.ciudad << endl;
        cout << "Email: " << persona.contacto.email << endl;
        cout << "Telefono: " << persona.contacto.telefono << endl;
        cout << "Puesto: " << persona.empleo.puesto << endl;
        cout << "Empresa: " << persona.empleo.empresa << endl;
        cout << "-------------------------" << endl;
    }

    return 0;
}
