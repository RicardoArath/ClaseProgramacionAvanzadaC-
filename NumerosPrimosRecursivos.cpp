#include <iostream>
#include <cmath>
#include <vector>

//Ricardo Arath Sanchez Aguirre 583928
//Desarrolla un programa que dado un número me calcule todos los números primos del 1 al número dado y los imprima en pantalla. 

using namespace std;

bool esPrimo(int n, int divisor = 2) {
    // Casos base
    if (n <= 1) return false;           // 1 y números menores no son primos
    if (n <= 3) return true;            // 2 y 3 son primos
    if (n % 2 == 0) return false;       // números pares mayores que 2 no son primos
    
    // Si el divisor es mayor que la raíz cuadrada, el número es primo
    if (divisor > sqrt(n)) return true;
    
    // Si n es divisible por el divisor actual, no es primo
    if (n % divisor == 0) return false;
    
    // Llamada recursiva con el siguiente divisor impar
    return esPrimo(n, divisor + 1);
}

void encontrarPrimos(int actual, int limite, vector<int>& primos){
    if (actual > limite) return;

    if (esPrimo(actual)) {
        primos.push_back(actual);
    }
    
    // Llamada recursiva con el siguiente número
    encontrarPrimos(actual + 1, limite, primos);
}


int main(){
    int numero;
    vector<int> primos; // Arreglo para guardar los numeros primos
    
    cout << "Ingrese un numero entero positivo: ";
    cin >> numero;

    if(numero < 1){
        cout << "El numero debe ser positivo y mayor que 0." << endl;
        return 1;
    }

    // Encontrar todos los primos del 2 al número dado usando recursión
    encontrarPrimos(2, numero, primos);
    
    // Imprimir los primos usando recursión
    cout << "Numeros primos del 1 al " << numero << ": ";
    for (int i = 0; i < primos.size(); i++) {
        cout << primos[i] << " ";
    }
    cout << endl;

    return 0;
}