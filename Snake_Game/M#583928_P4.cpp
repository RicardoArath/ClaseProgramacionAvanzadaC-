// Archivos de cabecera necesarios
#include <conio.h>
#include <iostream>
#include <windows.h>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <stdexcept>
using namespace std;

// Constantes para las dimensiones del tablero
const int ANCHO = 100;
const int ALTO = 100;

// Códigos de las teclas de flecha
#define TECLA_ARRIBA 72
#define TECLA_ABAJO 80
#define TECLA_IZQUIERDA 75
#define TECLA_DERECHA 77

// Coordenadas de la cabeza de la serpiente (eje-x, eje-y)
int cabezaX, cabezaY;

// Coordenadas de la comida
int comidaX, comidaY;

// Variable para almacenar la puntuación del jugador
int puntuacionJugador;

// Arreglos para almacenar las coordenadas de la cola de la serpiente
int colaX[10000], colaY[10000];

// Variable para almacenar la longitud de la cola de la serpiente
int longitudCola;

// Enumeración para las direcciones de movimiento de la serpiente
enum Direccion { PARAR = 0, IZQUIERDA, DERECHA, ARRIBA, ABAJO };

// Variable de dirección
Direccion dir;

// Variable booleana para verificar si el juego terminó
bool juegoTerminado;

// Variables para el sistema de comida temporal
chrono::steady_clock::time_point tiempoSpawnComida;
chrono::steady_clock::time_point ultimoIntentosSpawn;
bool comidaActiva;

// Función gotoxy para posicionar el cursor sin parpadeo
void gotoxy(int x, int y) {
    try {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hConsole == INVALID_HANDLE_VALUE) {
            throw runtime_error("Error al obtener el handle de la consola");
        }
        COORD posicion;
        posicion.X = x;
        posicion.Y = y;
        SetConsoleCursorPosition(hConsole, posicion);
    } catch (const exception& e) {
        cerr << "Error en gotoxy: " << e.what() << endl;
    }
}

// Función para establecer el color del texto
void establecerColor(int color) {
    try {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hConsole != INVALID_HANDLE_VALUE) {
            SetConsoleTextAttribute(hConsole, color);
        }
    } catch (const exception& e) {
        cerr << "Error al establecer color: " << e.what() << endl;
    }
}

// Función para ocultar el cursor
void ocultarCursor() {
    try {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(hConsole, &cursorInfo);
        cursorInfo.bVisible = false;
        SetConsoleCursorInfo(hConsole, &cursorInfo);
    } catch (const exception& e) {
        cerr << "Error al ocultar cursor: " << e.what() << endl;
    }
}

// Función para mostrar el cursor
void mostrarCursor() {
    try {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(hConsole, &cursorInfo);
        cursorInfo.bVisible = true;
        SetConsoleCursorInfo(hConsole, &cursorInfo);
    } catch (const exception& e) {
        cerr << "Error al mostrar cursor: " << e.what() << endl;
    }
}

// Función para ajustar el tamaño de la ventana de consola
void ajustarVentana() {
    try {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        
        // Configurar fuente pequeña
        CONSOLE_FONT_INFOEX cfi;
        cfi.cbSize = sizeof(cfi);
        cfi.nFont = 0;
        cfi.dwFontSize.X = 4;
        cfi.dwFontSize.Y = 6;
        cfi.FontFamily = FF_DONTCARE;
        cfi.FontWeight = FW_NORMAL;
        wcscpy_s(cfi.FaceName, L"Terminal");
        SetCurrentConsoleFontEx(hConsole, FALSE, &cfi);
        
        // Ajustar tamaño del buffer
        COORD nuevoTamano = {102, 105};
        SetConsoleScreenBufferSize(hConsole, nuevoTamano);
        
        // Ajustar tamaño de ventana
        SMALL_RECT ventana = {0, 0, 101, 104};
        SetConsoleWindowInfo(hConsole, TRUE, &ventana);
    } catch (const exception& e) {
        cerr << "Error al ajustar ventana: " << e.what() << endl;
    }
}

// Función para inicializar las variables del juego
void InicializarJuego() {
    try {
        juegoTerminado = false; // Estado inicial del juego
        dir = PARAR;    // Dirección inicial
        cabezaX = ANCHO / 2; // Posición inicial en X
        cabezaY = ALTO / 2; // Posición inicial en Y
        comidaX = rand() % ANCHO; // Posición inicial de la comida en X
        comidaY = rand() % ALTO; // Posición inicial de la comida en Y
        puntuacionJugador = 0; // Puntuación inicial
        longitudCola = 2; // Longitud inicial de la cola
        comidaActiva = true; // La comida comienza activa
        tiempoSpawnComida = chrono::steady_clock::now(); // Tiempo de spawn inicial
        ultimoIntentosSpawn = chrono::steady_clock::now(); // Tiempo del último intento de spawn
    } catch (const exception& e) {
        cerr << "Error al inicializar juego: " << e.what() << endl;
        throw;
    }
}

// Función para manejar la comida temporal
void GestionarComidaTemporal() {
    try {
        auto ahora = chrono::steady_clock::now();
        
        // Si la comida está activa, verificar si debe desaparecer (10 segundos)
        if (comidaActiva) {
            auto duracion = chrono::duration_cast<chrono::seconds>(ahora - tiempoSpawnComida).count();
            if (duracion >= 10) {   // Desaparece después de 10 segundos
                comidaActiva = false; // La comida desaparece
            }
        }
        
        // Si no hay comida, intentar crear una nueva cada 5 segundos
        if (!comidaActiva) {
            auto duracionUltimoIntento = chrono::duration_cast<chrono::seconds>(ahora - ultimoIntentosSpawn).count();
            if (duracionUltimoIntento >= 5) {
                comidaX = rand() % ANCHO;
                comidaY = rand() % ALTO;
                comidaActiva = true;
                tiempoSpawnComida = chrono::steady_clock::now();
                ultimoIntentosSpawn = ahora;
            }
        }
    } catch (const exception& e) {
        cerr << "Error en gestión de comida: " << e.what() << endl;
    }
}

// Función para renderizar el tablero del juego (solo primera vez)
void DibujarTableroInicial(string nombreJugador) {
    try {
        system("cls");
        
        // Dibujar encabezado
        establecerColor(11); // Cian brillante
        gotoxy(0, 0);
        cout << "+";
        for (int i = 0; i < ANCHO; i++) cout << "=";
        cout << "+";
        
        gotoxy(0, 1);
        cout << "| SNAKE GAME - 100x100 | Jugador: " << nombreJugador;
        int espacios = ANCHO - 35 - nombreJugador.length();
        for (int i = 0; i < espacios; i++) cout << " ";
        cout << "|";
        
        gotoxy(0, 2);
        cout << "+";
        for (int i = 0; i < ANCHO; i++) cout << "=";
        cout << "+";
        
        // Dibujar paredes laterales y espacios internos
        for (int i = 0; i < ALTO; i++) {
            gotoxy(0, i + 3);
            establecerColor(8); // Gris oscuro
            cout << "|";
            gotoxy(ANCHO + 1, i + 3);
            cout << "|";
        }
        
        // Dibujar borde inferior
        gotoxy(0, ALTO + 3);
        establecerColor(11);
        cout << "+";
        for (int i = 0; i < ANCHO; i++) cout << "=";
        cout << "+";
        
        establecerColor(7); // Restaurar color
    } catch (const exception& e) {
        cerr << "Error al dibujar tablero: " << e.what() << endl;
    }
}

// Función para actualizar solo los elementos que cambiaron
void ActualizarPantalla(string nombreJugador) {
    try {
        // Actualizar información del jugador
        gotoxy(0, 1);
        establecerColor(14); // Amarillo
        cout << "| Puntuacion: " << puntuacionJugador << " | Longitud: " << (longitudCola + 1) 
             << " | Jugador: " << nombreJugador;
        int espacios = ANCHO - 35 - nombreJugador.length() - to_string(puntuacionJugador).length() - to_string(longitudCola + 1).length();
        for (int i = 0; i < espacios; i++) cout << " ";
        cout << "|";
        
        // Dibujar la comida si está activa
        if (comidaActiva) {
            gotoxy(comidaX + 1, comidaY + 3);
            establecerColor(12); // Rojo brillante
            cout << "*";
        } else {
            // Borrar la comida si desapareció
            gotoxy(comidaX + 1, comidaY + 3);
            cout << " ";
        }
        
        // Dibujar la cabeza de la serpiente
        gotoxy(cabezaX + 1, cabezaY + 3);
        establecerColor(14); // Amarillo brillante
        cout << "O";
        
        // Dibujar la cola de la serpiente
        for (int i = 0; i < longitudCola; i++) {
            gotoxy(colaX[i] + 1, colaY[i] + 3);
            establecerColor(10); // Verde brillante
            cout << "o";
        }
        
        establecerColor(7); // Restaurar color
    } catch (const exception& e) {
        cerr << "Error al actualizar pantalla: " << e.what() << endl;
    }
}

// Función para actualizar el estado del juego
void ActualizarJuego() {
    try {
        // Guardar la antigua posición de la cabeza para borrarla después
        int antiguaCabezaX = cabezaX;
        int antiguaCabezaY = cabezaY;
        
        // Mover la cabeza según la dirección
        switch (dir) {
            case IZQUIERDA:
                cabezaX--;
                break;
            case DERECHA:
                cabezaX++;
                break;
            case ARRIBA:
                cabezaY--;
                break;
            case ABAJO:
                cabezaY++;
                break;
            default:
                break;
        }
        
        // Verificar colisión con las paredes
        if (cabezaX >= ANCHO || cabezaX < 0 || cabezaY >= ALTO || cabezaY < 0) {
            juegoTerminado = true;
            return;
        }
        
        // Verificar colisión con la cola
        for (int i = 0; i < longitudCola; i++) {
            if (colaX[i] == cabezaX && colaY[i] == cabezaY) {
                juegoTerminado = true;
                return;
            }
        }
        
        // Verificar colisión con la comida
        bool comio = false;
        if (comidaActiva && cabezaX == comidaX && cabezaY == comidaY) {
            puntuacionJugador += 10;
            longitudCola++;
            comidaActiva = false; // La comida desaparece al ser comida
            comio = true;
        }
        
        // Actualizar la cola
        // Guardar la última posición de la cola antes de actualizarla
        int ultimaColaX = -1;
        int ultimaColaY = -1;
        
        if (longitudCola > 0) {
            ultimaColaX = colaX[longitudCola - 1];
            ultimaColaY = colaY[longitudCola - 1];
            
            // Mover cada segmento de la cola hacia adelante
            for (int i = longitudCola - 1; i > 0; i--) {
                colaX[i] = colaX[i - 1];
                colaY[i] = colaY[i - 1];
            }
            
            // La primera posición de la cola ahora es donde estaba la cabeza
            colaX[0] = antiguaCabezaX;
            colaY[0] = antiguaCabezaY;
        }
        
        // Si no comió, borrar la última posición de la cola
        // (o la antigua cabeza si no tiene cola)
        if (!comio) {
            if (longitudCola > 0) {
                gotoxy(ultimaColaX + 1, ultimaColaY + 3);
                cout << " ";
            } else {
                // Si no tiene cola, borrar la antigua posición de la cabeza
                gotoxy(antiguaCabezaX + 1, antiguaCabezaY + 3);
                cout << " ";
            }
        }
        
        // Gestionar aparición/desaparición de comida
        GestionarComidaTemporal();
        
    } catch (const exception& e) {
        cerr << "Error al actualizar juego: " << e.what() << endl;
        throw;
    }
}

// Función para manejar la entrada del usuario
void EntradaUsuario() {
    try {
        // Verifica si se presionó una tecla
        if (_kbhit()) {
            int tecla = _getch();
            
            // Si es una tecla especial (flecha), leer el segundo byte
            if (tecla == 0 || tecla == 224) {
                tecla = _getch();
                
                switch (tecla) {
                    case TECLA_IZQUIERDA:
                        if (dir != DERECHA) dir = IZQUIERDA;
                        break;
                    case TECLA_DERECHA:
                        if (dir != IZQUIERDA) dir = DERECHA;
                        break;
                    case TECLA_ARRIBA:
                        if (dir != ABAJO) dir = ARRIBA;
                        break;
                    case TECLA_ABAJO:
                        if (dir != ARRIBA) dir = ABAJO;
                        break;
                }
            } else if (tecla == 'q' || tecla == 'Q' || tecla == 27) {
                // Tecla 'Q' o ESC para salir
                juegoTerminado = true;
            }
        }
    } catch (const exception& e) {
        cerr << "Error al procesar entrada: " << e.what() << endl;
    }
}

// Función para mostrar el mensaje de Game Over
void MostrarGameOver(string nombreJugador) {
    try {
        system("cls");
        establecerColor(12); // Rojo brillante
        cout << "\n+=======================================================+\n";
        cout << "|                      GAME OVER!                         |\n";
        cout << "+=========================================================+\n";
        
        establecerColor(14); // Amarillo
        cout << "|  Jugador: " << nombreJugador;
        int espacios = 45 - nombreJugador.length();
        for (int i = 0; i < espacios; i++) cout << " ";
        cout << "|\n";
        
        cout << "|  Puntuacion final: " << puntuacionJugador << " puntos";
        espacios = 31 - to_string(puntuacionJugador).length();
        for (int i = 0; i < espacios; i++) cout << " ";
        cout << "|\n";
        
        cout << "|  Longitud final: " << (longitudCola + 1) << " segmentos";
        espacios = 29 - to_string(longitudCola + 1).length();
        for (int i = 0; i < espacios; i++) cout << " ";
        cout << "|\n";
        
        establecerColor(12);
        cout << "+=======================================================+\n";
        establecerColor(7);
        cout << "\nPresiona cualquier tecla para salir...\n";
        
        mostrarCursor();
        _getch();
    } catch (const exception& e) {
        cerr << "Error al mostrar Game Over: " << e.what() << endl;
    }
}

// Función principal / bucle del juego
int main() {
    try {
        string nombreJugador;
        
        // Pantalla de bienvenida
        establecerColor(10); // Verde
        cout << "\n+================================================================+\n";
        cout << "|                                                                |\n";
        cout << "|                        SNAKE GAME                              |\n";
        cout << "|                      Tablero 100x100                           |\n";
        cout << "|                                                                |\n";
        cout << "+================================================================+\n";
        
        establecerColor(11); // Cian
        cout << "|  Instrucciones:                                                |\n";
        cout << "|  - Usa las FLECHAS del teclado para mover la serpiente        |\n";
        cout << "|  - Come la comida (*) para crecer y ganar puntos              |\n";
        cout << "|  - La comida aparece cada 5 segundos                          |\n";
        cout << "|  - La comida desaparece despues de 10 segundos                |\n";
        cout << "|  - Evita chocar contra las paredes o contra ti mismo          |\n";
        cout << "|  - Presiona 'Q' o ESC para salir del juego                    |\n";
        
        establecerColor(14); // Amarillo
        cout << "+================================================================+\n";
        cout << "|  Puntos por comida: +10                                        |\n";
        
        establecerColor(10);
        cout << "+================================================================+\n\n";
        
        establecerColor(7);
        cout << "Ingresa tu nombre: ";
        getline(cin, nombreJugador);
        
        if (nombreJugador.empty()) {
            nombreJugador = "Jugador";
        }
        
        cout << "\nPresiona cualquier tecla para comenzar...\n";
        _getch();
        
        // Inicializar generador de números aleatorios
        srand(static_cast<unsigned>(time(0)));
        
        // Ajustar ventana y ocultar cursor
        ajustarVentana();
        ocultarCursor();
        
        // Inicializar el juego
        InicializarJuego();
        
        // Dibujar el tablero inicial
        DibujarTableroInicial(nombreJugador);
        
        // Bucle principal del juego
        while (!juegoTerminado) {
            ActualizarPantalla(nombreJugador);
            EntradaUsuario();
            
            // Solo actualizar si la serpiente se está moviendo
            if (dir != PARAR) {
                ActualizarJuego();
            }
            
            // Delay según la dificultad (100ms = velocidad media)
            Sleep(100);
        }
        
        // Mostrar pantalla de Game Over
        MostrarGameOver(nombreJugador);
        
    } catch (const exception& e) {
        cerr << "\n[ERROR CRITICO] " << e.what() << endl;
        mostrarCursor();
        cout << "\nPresiona cualquier tecla para salir...\n";
        _getch();
        return 1;
    } catch (...) {
        cerr << "\n[ERROR CRITICO] Excepcion desconocida." << endl;
        mostrarCursor();
        cout << "\nPresiona cualquier tecla para salir...\n";
        _getch();
        return 1;
    }
    
    return 0;
}
