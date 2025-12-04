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
const int ANCHO = 40;
const int ALTO = 20;

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

// Variables para los lagos (2x2)
const int MAX_LAGOS = 10;  // Número máximo de lagos
int lagosX[MAX_LAGOS];     // Coordenada X de la esquina superior izquierda del lago
int lagosY[MAX_LAGOS];     // Coordenada Y de la esquina superior izquierda del lago
int numLagos = 0;          // Número actual de lagos
chrono::steady_clock::time_point ultimoLagoSpawn;

// Variable para activar/desactivar debug (cambiar a true para ver mensajes)
const bool DEBUG_ACTIVO = true;

// Función de debug para mostrar mensajes de depuración en la parte inferior
void debug(const string& msg) {
    if (DEBUG_ACTIVO) {
        // Posicionar cursor debajo del tablero (ALTO + 5 para dejar espacio)
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD posicion;
        posicion.X = 0;
        posicion.Y = ALTO + 5;
        SetConsoleCursorPosition(hConsole, posicion);
        
        // Limpiar línea anterior
        cout << string(60, ' ');
        
        // Volver a posicionar y mostrar mensaje
        posicion.Y = ALTO + 5;
        SetConsoleCursorPosition(hConsole, posicion);
        SetConsoleTextAttribute(hConsole, 13); // Magenta
        cout << "[DEBUG] " << msg;
        SetConsoleTextAttribute(hConsole, 7); // Restaurar color
    }
}

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
        // Configurar fuente legible
        CONSOLE_FONT_INFOEX cfi;
        cfi.cbSize = sizeof(cfi);
        cfi.nFont = 0;
        cfi.dwFontSize.X = 8;
        cfi.dwFontSize.Y = 16;
        cfi.FontFamily = FF_DONTCARE;
        cfi.FontWeight = FW_NORMAL;
        wcscpy_s(cfi.FaceName, L"Consolas");
        SetCurrentConsoleFontEx(hConsole, FALSE, &cfi);
        
        // Ajustar tamaño del buffer (ANCHO+2 x ALTO+6)
        COORD nuevoTamano = {50, 30};
        SetConsoleScreenBufferSize(hConsole, nuevoTamano);
        
        // Ajustar tamaño de ventana
        SMALL_RECT ventana = {0, 0, 49, 29};
        SetConsoleWindowInfo(hConsole, TRUE, &ventana);
    } catch (const exception& e) {
        cerr << "Error al ajustar ventana: " << e.what() << endl;
    }
}

// Función para verificar si una posición colisiona con un lago
bool colisionaConLago(int x, int y) {
    for (int i = 0; i < numLagos; i++) {
        // El lago ocupa 2x2: (lagosX, lagosY), (lagosX+1, lagosY), (lagosX, lagosY+1), (lagosX+1, lagosY+1)
        if (x >= lagosX[i] && x <= lagosX[i] + 1 &&
            y >= lagosY[i] && y <= lagosY[i] + 1) {
            return true;
        }
    }
    return false;
}

// Función para generar un nuevo lago en posición aleatoria
void generarLago() {
    if (numLagos >= MAX_LAGOS) return;
    
    int intentos = 0;
    while (intentos < 100) {
        // Generar posición aleatoria (dejando margen para el 2x2 completo)
        int nuevoX = 1 + rand() % (ANCHO - 4);  // 1 a ANCHO-4
        int nuevoY = 1 + rand() % (ALTO - 4);   // 1 a ALTO-4
        
        // Verificar que no esté cerca de la serpiente ni de otros lagos
        bool posicionValida = true;
        
        // No generar cerca de la cabeza de la serpiente
        if (abs(nuevoX - cabezaX) < 5 && abs(nuevoY - cabezaY) < 5) {
            posicionValida = false;
        }
        
        // No generar sobre la cola
        for (int i = 0; i < longitudCola && posicionValida; i++) {
            if (abs(nuevoX - colaX[i]) < 2 && abs(nuevoY - colaY[i]) < 2) {
                posicionValida = false;
            }
        }
        
        // No generar sobre otros lagos
        for (int i = 0; i < numLagos && posicionValida; i++) {
            if (abs(nuevoX - lagosX[i]) < 4 && abs(nuevoY - lagosY[i]) < 4) {
                posicionValida = false;
            }
        }
        
        // No generar sobre la comida
        if (abs(nuevoX - comidaX) < 3 && abs(nuevoY - comidaY) < 3) {
            posicionValida = false;
        }
        
        if (posicionValida) {
            lagosX[numLagos] = nuevoX;
            lagosY[numLagos] = nuevoY;
            numLagos++;
            debug("Nuevo lago generado en (" + to_string(nuevoX) + ", " + to_string(nuevoY) + ") - Total: " + to_string(numLagos));
            return;
        }
        
        intentos++;
    }
}

// Función para inicializar las variables del juego
void InicializarJuego() {
    try {
        debug("Inicializando variables del juego...");
        juegoTerminado = false; // Estado inicial del juego
        dir = DERECHA;    // Dirección inicial
        cabezaX = ANCHO / 2; // Posición inicial en X
        cabezaY = ALTO / 2; // Posición inicial en Y
        
        // Generar comida inicial en posición segura (dentro del área jugable)
        comidaX = 2 + rand() % (ANCHO - 4);  // 2 a ANCHO-3
        comidaY = 2 + rand() % (ALTO - 4);   // 2 a ALTO-3
        comidaActiva = true; // La comida SÍ comienza activa
        
        puntuacionJugador = 0; // Puntuación inicial
        longitudCola = 2; // Longitud inicial de la cola
        tiempoSpawnComida = chrono::steady_clock::now(); // Tiempo de spawn inicial
        ultimoIntentosSpawn = chrono::steady_clock::now(); // Tiempo del último intento de spawn
        
        // Inicializar lagos
        numLagos = 0;
        ultimoLagoSpawn = chrono::steady_clock::now();
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
                debug("Comida desaparecida por tiempo");
                comidaActiva = false; // La comida desaparece
            }
        }
        
        // Si no hay comida, intentar crear una nueva cada 3 segundos
        if (!comidaActiva) {
            auto duracionUltimoIntento = chrono::duration_cast<chrono::seconds>(ahora - ultimoIntentosSpawn).count();
            if (duracionUltimoIntento >= 3) {
                comidaX = 2 + rand() % (ANCHO - 4);  // 2 a ANCHO-3
                comidaY = 2 + rand() % (ALTO - 4);   // 2 a ALTO-3
                comidaActiva = true;
                debug("Nueva comida generada en (" + to_string(comidaX) + ", " + to_string(comidaY) + ")");
                tiempoSpawnComida = chrono::steady_clock::now();
                ultimoIntentosSpawn = ahora;
            }
        }
    } catch (const exception& e) {
        cerr << "Error en gestión de comida: " << e.what() << endl;
    }
}

// Función para gestionar la aparición de nuevos lagos
void GestionarLagos() {
    try {
        auto ahora = chrono::steady_clock::now();
        auto duracion = chrono::duration_cast<chrono::seconds>(ahora - ultimoLagoSpawn).count();
        
        // Agregar un nuevo lago después de 5 segundos (primeros 3), luego cada 15 segundos
        int tiempoRequerido = (numLagos < 3) ? 5 : 15;
        if (duracion >= tiempoRequerido && numLagos < MAX_LAGOS) {
            generarLago();
            ultimoLagoSpawn = ahora;
        }
    } catch (const exception& e) {
        cerr << "Error en gestión de lagos: " << e.what() << endl;
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
        cout << "| Puntuacion: " << puntuacionJugador << " | Longitud: " << (longitudCola + 1) << " | Jugador: " << nombreJugador;
        int espacios = ANCHO - 35 - nombreJugador.length() - to_string(puntuacionJugador).length() - to_string(longitudCola + 1).length();
        for (int i = 0; i < espacios; i++) cout << " ";
        cout << "|";
        
        // Dibujar los lagos (2x2 en azul) - solo si están dentro del área válida
        establecerColor(9); // Azul brillante
        for (int i = 0; i < numLagos; i++) {
            // Verificar que el lago esté dentro del área de juego
            if (lagosX[i] >= 0 && lagosX[i] < ANCHO - 1 && 
                lagosY[i] >= 0 && lagosY[i] < ALTO - 1) {
                // Dibujar el lago 2x2
                gotoxy(lagosX[i] + 1, lagosY[i] + 3);
                cout << "~~";
                gotoxy(lagosX[i] + 1, lagosY[i] + 4);
                cout << "~~";
            }
        }
        
        // Dibujar la comida si está activa y dentro del área válida
        if (comidaActiva && comidaX >= 0 && comidaX < ANCHO && 
            comidaY >= 0 && comidaY < ALTO) {
            gotoxy(comidaX + 1, comidaY + 3);
            establecerColor(12); // Rojo brillante
            cout << "*";
        } else if (!comidaActiva && comidaX >= 0 && comidaX < ANCHO && 
                   comidaY >= 0 && comidaY < ALTO) {
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
            debug("Colision con pared detectada");
            juegoTerminado = true;
            return;
        }
        
        // Verificar colisión con los lagos
        if (colisionaConLago(cabezaX, cabezaY)) {
            debug("Colision con lago detectada");
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
            debug("Serpiente comio! Puntuacion: " + to_string(puntuacionJugador + 10));
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
        
        // Gestionar aparición de nuevos lagos
        GestionarLagos();
        
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
        establecerColor(9); // Azul
        cout << "|  - CUIDADO con los LAGOS (~~) - Si los pisas, pierdes!        |\n";
        cout << "|  - Los lagos aparecen cada 15 segundos                        |\n";
        establecerColor(11); // Cian
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
