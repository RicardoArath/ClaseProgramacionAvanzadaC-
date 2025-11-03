#include <iostream>
#include <windows.h>

using namespace std;

void imprimirTablero(char tablero[10][10], int ancho = 50, int alto = 50) {
    //HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    //SetConsoleTextAttribute(hConsole, 1); Cambia de color el texto


    system("cls"); // Limpiar la consola (funciona en Windows)
    for (int i = 0; i < alto; i++) {
        cout << "| ";
        for (int j = 0; j < ancho; j++) {
            if (tablero[i][j] != '\0')
            cout << i;
        }
        cout << " |"<< endl;
    }
}

void movimiento(){

}


void snakeGame(){
    try {
        char tablero[10][10];
        int ancho = 10;
        int alto = 10;
        
        //inicializamos el tablero en 0
        for (int i = 0; i < alto; i++)
            for (int j = 0; j < ancho; j++)
                tablero[i][j] == i;

        imprimirTablero(tablero, ancho, alto);

        while (true) {
            movimiento();
        }

    }catch(const exception& e){
        cerr << "[ERROR] Excepcion capturada en main: " << e.what() << endl;
    }

}

int main(){
    cout << "SNAKE GAME" << endl;
    cout << "Press 'q' to quit the game." << endl;

    cout << "Ready to play? Press any key to start..." << endl;
        cin;

    snakeGame();

    return 0;
}