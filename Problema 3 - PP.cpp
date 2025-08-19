#include "iostream"
#include <string>
#include <cctype>
using namespace std;

/*Ejercicio 3: Verificar si una string es un palindromo
Un palindromo es una palabra o frase que se lee igual de adelante o hacia atras hacia adelante (ej. oso, reconocer). Escribe un programa que
pida al usuario una palabra y determine si es un palindromo
*/

int main() {    
    string palabra, limpia = "";
    string palindromo = "";

    cout<<"Ingresa una palabra o frase: "; 
    getline(cin,palabra);

    for(char c : palabra){
        if(!isspace(c)){
            limpia += tolower(c);
        }
    }

    for(int i = limpia.length() - 1; i >= 0; i--) {
        palindromo += limpia[i];
    }

    cout << "Pusiste " << palindromo << "\n";

    if(palindromo == limpia){
        cout << "La palabra es palindromo";
    } else {
        cout << "La palabra NO es palindromo";
    }
}