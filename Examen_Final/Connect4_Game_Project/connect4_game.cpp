#include <iostream>
#include <windows.h>

using namespace std;

const int FILAS = 6;
const int COLUMNAS = 7;

// Colores para la consola
const int COLOR_ROJO = 12;     // Rojo brillante
const int COLOR_AMARILLO = 14; // Amarillo
const int COLOR_BLANCO = 7;    // Blanco (default)
const int COLOR_CYAN = 11;     // Cyan brillante

// Función para establecer el color del texto
void establecerColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

string debug(const string& msg) {
    // Placeholder for debug function
    return msg;
}

void imprimirTablero(char tablero[FILAS][COLUMNAS]){
    cout << debug("Imprimiendo el tablero del juego...");
    
    // Imprimir números de columna
    establecerColor(COLOR_CYAN);
    cout << "\n ";
    for (int j = 1; j <= COLUMNAS; ++j) {
        cout << " " << j;
    }
    cout << endl;
    
    establecerColor(COLOR_BLANCO);
    
    // Imprimir el tablero
    for (int i = 0; i < FILAS; ++i) {
        cout << "|";
        for (int j = 0; j < COLUMNAS; ++j) {
            if (tablero[i][j] == 'R') {
                establecerColor(COLOR_ROJO);
                cout << "O";
                establecerColor(COLOR_BLANCO);
            } else if (tablero[i][j] == 'A') {
                establecerColor(COLOR_AMARILLO);
                cout << "O";
                establecerColor(COLOR_BLANCO);
            } else {
                cout << " ";
            }
            cout << "|";
        }
        cout << endl;
    }
    establecerColor(COLOR_CYAN);
    cout << "---------------" << endl;
    establecerColor(COLOR_BLANCO);
}

// Verificar si hay un ganador
bool verificarGanador(char tablero[FILAS][COLUMNAS], char ficha) {
    // Verificar horizontales
    for (int i = 0; i < FILAS; ++i) {
        for (int j = 0; j <= COLUMNAS - 4; ++j) {
            if (tablero[i][j] == ficha && tablero[i][j+1] == ficha &&
                tablero[i][j+2] == ficha && tablero[i][j+3] == ficha) {
                return true;
            }
        }
    }
    
    // Verificar verticales
    for (int i = 0; i <= FILAS - 4; ++i) {
        for (int j = 0; j < COLUMNAS; ++j) {
            if (tablero[i][j] == ficha && tablero[i+1][j] == ficha &&
                tablero[i+2][j] == ficha && tablero[i+3][j] == ficha) {
                return true;
            }
        }
    }
    
    // Verificar diagonal descendente (\)
    for (int i = 0; i <= FILAS - 4; ++i) {
        for (int j = 0; j <= COLUMNAS - 4; ++j) {
            if (tablero[i][j] == ficha && tablero[i+1][j+1] == ficha &&
                tablero[i+2][j+2] == ficha && tablero[i+3][j+3] == ficha) {
                return true;
            }
        }
    }
    
    // Verificar diagonal ascendente (/)
    for (int i = 3; i < FILAS; ++i) {
        for (int j = 0; j <= COLUMNAS - 4; ++j) {
            if (tablero[i][j] == ficha && tablero[i-1][j+1] == ficha &&
                tablero[i-2][j+2] == ficha && tablero[i-3][j+3] == ficha) {
                return true;
            }
        }
    }
    
    return false;
}

// Verificar si hay empate (tablero lleno)
bool verificarEmpate(char tablero[FILAS][COLUMNAS]) {
    for (int j = 0; j < COLUMNAS; ++j) {
        if (tablero[0][j] == ' ') {
            return false; // Aún hay espacio
        }
    }
    return true;
}

void juego() {
    cout << debug("Iniciando el juego de Conecta 4...");
    
    char tablero[FILAS][COLUMNAS];
    
    // Inicializar el tablero con espacios vacíos
    for (int i = 0; i < FILAS; ++i) {
        for (int j = 0; j < COLUMNAS; ++j) {
            tablero[i][j] = ' ';
        }
    }
    
    int turno = 0; // 0 para jugador 1, 1 para jugador 2
    bool juegoTerminado = false;
    
    // Imprimir tablero inicial
    imprimirTablero(tablero);
    
    while (!juegoTerminado) {
        char fichaActual = (turno == 0) ? 'R' : 'A';  // R = Rojo, A = Amarillo
        string nombreJugador = (turno == 0) ? "ROJO" : "AMARILLO";
        int colorJugador = (turno == 0) ? COLOR_ROJO : COLOR_AMARILLO;
        int columna;
        
        establecerColor(colorJugador);
        cout << "Jugador " << nombreJugador;
        establecerColor(COLOR_BLANCO);
        cout << ", ingresa el numero de columna (1-7): ";
        cin >> columna;
        
        // Validar entrada
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Entrada invalida. Ingresa un numero del 1 al 7." << endl;
            continue;
        }
        
        if (columna < 1 || columna > 7) {
            cout << "Columna invalida. Intenta de nuevo." << endl;
            continue;
        }
        
        // Colocar la ficha en la columna seleccionada
        bool colocado = false;
        for (int i = FILAS - 1; i >= 0; --i) {
            if (tablero[i][columna - 1] == ' ') {
                tablero[i][columna - 1] = fichaActual;
                colocado = true;
                break;
            }
        }
        
        if (!colocado) {
            cout << "La columna esta llena. Intenta de nuevo." << endl;
            continue;
        }
        
        // Imprimir el tablero actualizado
        imprimirTablero(tablero);
        
        // Verificar si hay ganador
        if (verificarGanador(tablero, fichaActual)) {
            cout << "\n";
            establecerColor(colorJugador);
            cout << "*** FELICIDADES! Jugador " << nombreJugador << " ha ganado! ***" << endl;
            establecerColor(COLOR_BLANCO);
            juegoTerminado = true;
            break;
        }
        
        // Verificar empate
        if (verificarEmpate(tablero)) {
            cout << "\n*** EMPATE! El tablero esta lleno. ***" << endl;
            juegoTerminado = true;
            break;
        }
        
        // Cambiar de turno
        turno = 1 - turno;
    }
    
    cout << "\nGracias por jugar Conecta 4!" << endl;
}

int main() {
    try {
        establecerColor(COLOR_CYAN);
        cout << "==============================" << endl;
        cout << "   Bienvenido a Conecta 4!" << endl;
        cout << "==============================" << endl;
        establecerColor(COLOR_BLANCO);
        cout << "\nObjetivo: Alinear 4 fichas en linea (horizontal, vertical o diagonal)." << endl;
        cout << "Jugador ";
        establecerColor(COLOR_ROJO);
        cout << "ROJO";
        establecerColor(COLOR_BLANCO);
        cout << " vs Jugador ";
        establecerColor(COLOR_AMARILLO);
        cout << "AMARILLO" << endl;
        establecerColor(COLOR_BLANCO);
        cout << "Ingresa el numero de columna (1-7) para colocar tu ficha." << endl;
        cout << "\nPresiona Enter para comenzar..." << endl;
        cin.get();

        juego();
        
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
    }

    return 0;
}