#include "iostream"
#include <string>
using namespace std;

/*Ejercicio 1: Contar vocales en una string
Escribe un programa que pida al usuario que ingrese una palabra o frase y luego
cuente cuantas vocales (a,e,i,o,u) hay en ella, sin importar si son mayusculas o minusculas
*/

int main() {
    string palabra = "";
    int vocales = 0;
    
    cout<<"Ingresa una palabra o frase: "; 
    getline(cin,palabra);

    for(int i=0; i < palabra.length(); i++){
        char c = tolower(palabra[i]);
        if(c == 'a'||c == 'e'||c == 'i'||c == 'o'||c == 'u'){
            vocales++;
        }
    }
    
    cout << "Hay " << vocales << " vocales en tu palabra o frase.\n";
    
    return 0;
}