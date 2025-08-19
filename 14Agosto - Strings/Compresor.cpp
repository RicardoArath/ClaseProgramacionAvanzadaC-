/*COMPRESOR DE CARACTERES Y CODIGO*/
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main(){

    string texto;
    cout << "Ingrese un texto: /n";
    cin >> texto;

    int numCaracteres = texto.length();
    string car[];

    vector<vector<string, int>> caracteres;
    //vector <vector<string>> caracteres;
    string textoComprimido;

    for(int i=0; i < numCaracteres; i++){
        texto[i] = tolower(texto[i]);
    }

    /*Ejemplo: "Mar"
    caracteres[0][0] = {"m"}
    caracteres[0][1] = {1}
    caracteres[1][0] = {"a"}
    caracteres[1][1] = {2}
    caracteres[2][0] = {"a"}
    caracteres[2][1] = {2}
    */
    for(int i = 0; i < numCaracteres; i++){
        for(int j = 0; j < i; j++){
            if(texto[i] == caracteres[j][0]){
                caracteres[j][1] = caracteres[j][1] + 1;
            } else {
                caracteres[j][0] += texto[i];
            }
       }
    }

    cout << "La compresion resulto en: " << textoComprimido << endl;
    return 0;
}

/*


caracteres = [0][0] = {"a"}
caracteres = [0][1] = {1}
caracteres = [0] = {"a", 1}


*/