#include "iostream"
#include <string>
using namespace std;
//Codigo morse. convertir palabras a codigo morse y viceversa.

int main(){
    string frase;
    string morse[] = {".-", "-...", "-.-.","-..",".","..-.","--.","....","..",".---","-.-",".-..","--","-.","---",".--.","--.-",".-.","...","-","..-","...-",".--","-..-","-.--","--.."};
    string abecedario[] = {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"};

    cout << "Dame una palabra o un texto en codigo morse: " << endl;
    getline(cin,frase);

    //PALABRA A CODIGO MORSE
    string palabraMorse;
    for(int i = 0; i < frase.length();i++){
        frase[i] = tolower(frase[i]);
        if(frase[i] == 'ñ'){
            palabraMorse += "--.-- "; // Morse real de la ñ
            continue;
        }
        if(frase[i] == ' '){
            palabraMorse+="   ";
        } else {
            for(int j = 0; j < 26; j++){
                if(frase[i] == abecedario[j][0]){
                    palabraMorse+=morse[j] + " ";
                    break;
                }
            }   
        }
    }

    cout << "La palabra en codigo Morse es: " << palabraMorse << endl;

    return 0;
}