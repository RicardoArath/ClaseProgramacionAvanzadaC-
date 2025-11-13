#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;

// Códigos de las teclas de flecha
#define KEY_UP    72
#define KEY_DOWN  80
#define KEY_LEFT  75
#define KEY_RIGHT 77

void debug(string mensaje){
    cout << "[DEBUG]: " << mensaje << endl;
}

// Función para establecer el color del texto
void establecerColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

// Función para limpiar la pantalla
void limpiarPantalla() {
    system("cls");
}

// Función para imprimir el tablero con ASCII extendido
void imprimirTablero(const vector<int>& tablero) {
    limpiarPantalla();
    
    establecerColor(11); // Cian brillante
    cout << "\n\xC9\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xBB\n";
    cout << "\xBA      PUZZLE 15 - Ordenar Numeros     \xBA\n";
    cout << "\xCC\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xB9\n";
    establecerColor(7);
    
    cout << "\xBA  Controles: W/A/S/D o Flechas        \xBA\n";
    cout << "\xBA  Salir: Q o ESC                      \xBA\n";
    
    establecerColor(11);
    cout << "\xC8\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xBC\n\n";
    
    // Imprimir el tablero con bordes ASCII
    establecerColor(14); // Amarillo
    cout << "  \xC9\xCD\xCD\xCD\xCB\xCD\xCD\xCD\xCB\xCD\xCD\xCD\xCB\xCD\xCD\xCD\xBB\n";
    
    for (int i = 0; i < 4; ++i) {
        cout << "  \xBA";
        for (int j = 0; j < 4; ++j) {
            if (tablero[i * 4 + j] == 0) {
                establecerColor(8); // Gris oscuro para espacio vacío
                cout << "   ";
            } else {
                establecerColor(10); // Verde brillante para números
                if (tablero[i * 4 + j] < 10)
                    cout << " " << tablero[i * 4 + j] << " ";
                else
                    cout << tablero[i * 4 + j] << " ";
            }
            establecerColor(14);
            cout << "\xBA";
        }
        cout << "\n";
        
        if (i < 3) {
            cout << "  \xCC\xCD\xCD\xCD\xCE\xCD\xCD\xCD\xCE\xCD\xCD\xCD\xCE\xCD\xCD\xCD\xB9\n";
        }
    }
    
    cout << "  \xC8\xCD\xCD\xCD\xCA\xCD\xCD\xCD\xCA\xCD\xCD\xCD\xCA\xCD\xCD\xCD\xBC\n";
    establecerColor(7);
}

void randomizarNumeros(vector<int>& numbers) {
    numbers.clear();
    for (int i = 1; i <= 15; ++i) numbers.push_back(i);
    numbers.push_back(0); // espacio vacío

    // Semilla basada en el tiempo actual
    unsigned seed = chrono::system_clock::now().time_since_epoch().count(); // Obtener semilla actual
    static default_random_engine g(seed); // static evita reseed en llamadas repetidas

    shuffle(numbers.begin(), numbers.end(), g); // Mezclar los números

}

// Función para encontrar la posición del espacio vacío (0)
int encontrarEspacioVacio(const vector<int>& tablero) {
    for (int i = 0; i < 16; ++i) {
        if (tablero[i] == 0) return i;
    }
    return -1;
}

// Función para mover el espacio vacío
bool moverEspacio(vector<int>& tablero, char direccion) {
    int posVacio = encontrarEspacioVacio(tablero);
    int fila = posVacio / 4;
    int columna = posVacio % 4;
    int nuevaPos = -1;
    
    try {
        switch(direccion) {
            case 'w': // Arriba - mover número de arriba hacia abajo
                if (fila > 0) nuevaPos = posVacio - 4;
                break;
            case 's': // Abajo - mover número de abajo hacia arriba
                if (fila < 3) nuevaPos = posVacio + 4;
                break;
            case 'a': // Izquierda - mover número de izquierda hacia derecha
                if (columna > 0) nuevaPos = posVacio - 1;
                break;
            case 'd': // Derecha - mover número de derecha hacia izquierda
                if (columna < 3) nuevaPos = posVacio + 1;
                break;
            default:
                return false;
        }
        
        if (nuevaPos != -1) {
            swap(tablero[posVacio], tablero[nuevaPos]);
            debug("Movimiento realizado");
            return true;
        }
    } catch (const exception& e) {
        cerr << "Error al mover: " << e.what() << endl;
    }
    
    return false;
}

// Función para verificar si el tablero está ordenado
bool estaOrdenado(const vector<int>& tablero) {
    for (int i = 0; i < 15; ++i) {
        if (tablero[i] != i + 1) return false;
    }
    return tablero[15] == 0; // El último debe ser el espacio vacío
}

// Función para manejar la entrada del usuario (WASD o Flechas)
char obtenerEntrada() {
    if (_kbhit()) {
        int tecla = _getch();
        
        // Convertir mayúsculas a minúsculas
        if (tecla >= 'A' && tecla <= 'Z') {
            tecla = tecla + 32;
        }
        
        // Teclas normales (WASD, Q, ESC)
        if (tecla == 'w' || tecla == 'a' || tecla == 's' || tecla == 'd' || 
            tecla == 'q' || tecla == 27) {
            return tecla;
        }
        
        // Teclas especiales (flechas)
        if (tecla == 0 || tecla == 224) {
            tecla = _getch();
            switch (tecla) {
                case KEY_UP:    return 'w';
                case KEY_DOWN:  return 's';
                case KEY_LEFT:  return 'a';
                case KEY_RIGHT: return 'd';
            }
        }
    }
    
    return 0; // No se presionó ninguna tecla válida
}

int main(){
    try {
        vector<int> numbers; // 0 representa el espacio vacío
        randomizarNumeros(numbers);

        bool juegoActivo = true;
        int movimientos = 0;
        
        // Pantalla de bienvenida
        establecerColor(10);
        cout << "\n\xC9\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xBB\n";
        cout << "\xBA                                            \xBA\n";
        cout << "\xBA          PUZZLE 15 - JUEGO CLASICO         \xBA\n";
        cout << "\xBA                                            \xBA\n";
        cout << "\xC8\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xBC\n";
        establecerColor(11);
        cout << "\nObjetivo: Ordenar los numeros del 1 al 15\n";
        cout << "Usa W/A/S/D o las flechas para mover\n";
        cout << "Presiona Q o ESC para salir\n\n";
        establecerColor(7);
        cout << "Presiona cualquier tecla para comenzar...\n";
        _getch();
        
        // Loop principal del juego
        while (juegoActivo) {
            imprimirTablero(numbers);
            
            establecerColor(13); // Magenta
            cout << "\n  Movimientos: " << movimientos << "\n";
            establecerColor(7);
            
            // Verificar si ganó
            if (estaOrdenado(numbers)) {
                establecerColor(10);
                cout << "\n\xC9\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xBB\n";
                cout << "\xBA          \xFE FELICIDADES! \xFE GANASTE!          \xBA\n";
                cout << "\xBA      Completaste el puzzle en " << movimientos << " movimientos     \xBA\n";
                cout << "\xC8\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xBC\n";
                establecerColor(7);
                cout << "\nPresiona cualquier tecla para salir...\n";
                _getch();
                break;
            }
            
            // Obtener entrada del usuario
            char tecla = obtenerEntrada();
            
            if (tecla == 'q' || tecla == 27) {
                establecerColor(12);
                cout << "\n\xFE Gracias por jugar!\n";
                establecerColor(7);
                juegoActivo = false;
            } else if (tecla == 'w' || tecla == 'a' || tecla == 's' || tecla == 'd') {
                if (moverEspacio(numbers, tecla)) {
                    movimientos++;
                } else {
                    // Movimiento inválido (fuera de límites)
                    Beep(500, 100); // Sonido de error
                }
            }
            
            Sleep(50); // Pequeña pausa para no sobrecargar la CPU
        }
        
    } catch (const exception& e) {
        establecerColor(12);
        cerr << "\n[ERROR]: " << e.what() << endl;
        establecerColor(7);
        return 1;
    }

    return 0;
}