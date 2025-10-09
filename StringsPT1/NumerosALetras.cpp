/*Ricardo Arath Sanchez Aguirre
583928

Realiza un pequeño desarrollo de C++ utilizando las características de string.

Objetivo:  Convertir de números a letras de la misma forma en que se hace en las facturas y documentos comunes. 

1000000      un millon de pesos
1532.           mil quinientros treinta y dos pesos
1000.25       mil pesos 25/100

*/

#include <iostream>
#include <string>
#include <cmath>
using namespace std;

string numApalabra(int numeroDado) {
    string unidades[]   = {"cero", "uno","dos","tres","cuatro","cinco","seis","siete","ocho","nueve"};
    string decenas[]    = {"", "diez","veinte","treinta","cuarenta","cincuenta","sesenta","setenta","ochenta","noventa"};
    string especiales[] = {"diez","once","doce","trece","catorce","quince","dieciséis","diecisiete","dieciocho","diecinueve"};
    string centenas[]   = {"", "ciento","doscientos","trescientos","cuatrocientos","quinientos","seiscientos","setecientos","ochocientos","novecientos"};

    if (numeroDado < 10) return unidades[numeroDado];
    if (numeroDado < 20) return especiales[numeroDado - 10];
    if (numeroDado < 100) return decenas[numeroDado / 10] + (numeroDado % 10 != 0 ? " y " + numApalabra(numeroDado % 10) : "");
    if (numeroDado == 100) return "cien";
    if (numeroDado < 1000) return centenas[numeroDado / 100] + (numeroDado % 100 != 0 ? " " + numApalabra(numeroDado % 100) : "");
    if (numeroDado < 1000000) {
        string miles = numApalabra(numeroDado / 1000);
        if (miles == "uno") miles = "un";
        return miles + " mil" + (numeroDado % 1000 != 0 ? " " + numApalabra(numeroDado % 1000) : "");
    }

    return "Numero demasiado grande para convertir";
}

int main() {
    double num;
    cout << "Ingrese un monto (con o sin centavos): ";
    cin >> num;

    int parteEntera = (int)num;
    int centavos = round((num - parteEntera) * 100);

    string texto = numApalabra(parteEntera);
    if (parteEntera == 1) texto = "un";

    cout << texto;

    // Mostrar centavos solo si hay
    if (centavos > 0){
        cout << " " << (centavos < 10 ? "0" : "") << centavos << "/100";
    }

    // Peso o pesos
    cout << " " << (parteEntera == 1 ? "peso" : "pesos") << endl;

    return 0;
}
