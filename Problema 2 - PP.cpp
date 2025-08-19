#include "iostream"
#include <string>
using namespace std;

/*Ejercicio 2 : Invertir una string
Crea un programa que lea una palabra y muestre su version invertida. 
Por ejemplo, si el usuario ingresa "hola", el programa debe mostrar "aloh".
*/

int main() {
    string palabra;
    cout << "Ingrese una palabra: ";
    cin >> palabra;

    cout << "La palabra invertida es: ";
    for(int i = palabra.length(); i >= 0; i--){
        cout << palabra[i];
    }
}