#include "iostream"
#include <string>
#include <cctype>
using namespace std;

/*Ejercicio 5:Contar la ocurrencia de un caracter
Solicita al usuario una string y un caracter. El programa debe
contar cuantas veces aparece ese caracter especifico en la string.
*/

int main(){
    string palabra;
    string limpia;
    char caracter;

    cout << "Ingrese una palabra: \n";
    cin >> palabra;

    cout << "Ingrese un caracter: \n";
    cin >> caracter;

    int contador=0;

    for(char c : palabra){
        limpia += tolower(c);
    }

    caracter = tolower(caracter);

    for(int i = limpia.length() - 1; i >= 0; i--) {
        if(limpia[i] == caracter){
            contador++;
        }
    }
    
    cout<<"El caracter '"<<caracter<<"' se repite "<<contador<<" veces.";

    return 0;
}
