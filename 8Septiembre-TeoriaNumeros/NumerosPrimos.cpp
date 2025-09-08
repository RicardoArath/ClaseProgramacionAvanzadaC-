#include <iostream>
#include <cmath>
#include <vector>

//Ricardo Arath Sanchez Aguirre 583928
//Desarrolla un programa que dado un número me calcule todos los números primos del 1 al número dado y los imprima en pantalla. 

using namespace std;

int main(){
    int numero;
    vector<int> primos; //Arreglo para guardar los numeros primos
    cout << "Ingrese un numero entero positivo: ";
    cin >> numero;

    if(numero < 1){
        cout << "El numero debe ser positivo y mayor que 0." << endl;
        return 1;
    }

    for(int i = 1; i <= numero; i++)
    {
        bool esPrimo = true;
        for(int j = 2; j <= sqrt(i); j++)
        {   
            if(i % j == 0)
            {
                esPrimo = false;
                break;
            }
        }
        if(esPrimo)
        {
            primos.push_back(i);
        }
    }

    for(int i = 0; i < primos.size(); i++)
    {
        cout << primos[i] << " ";
    }
    cout << endl;

    return 0;
}