#include "iostream"
#include <string>
using namespace std;

/*Ejercicio 4: Concatenar strings
Pide al usuario que ingrese su nombre y su apellido por separado.
Luego, concatena ambas strings para formar un nombre completo y 
muestralo en pantalla.
*/

    int main() {   
        string nombre;
        string apellido;
        string nombreCompleto;

        cout << "Ingrese su nombre: \n";
        getline(cin, nombre); 
        
        cout << "Ingrese su apellido: \n";
        getline(cin, apellido);

        nombreCompleto += nombre +" "+ apellido;

        cout << "El nombre completo es " << nombreCompleto;

        return 0;
    }