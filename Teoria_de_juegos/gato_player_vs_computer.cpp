#include <iostream>
#include <map>
#include <vector>
#include <limits>

using namespace std;

bool debug = false;

void debugPrint(const string& mensaje) {
    if (debug) cout << "[DEBUG] " << mensaje << endl;
}

void imprimirTablero(const map<pair<int,int>, char>& tablero){
    cout << "\n       1      2      3" << endl;
    cout << "   __________________" << endl;
    
    for(int fila = 1; fila <= 3; fila++){
        cout << fila << "  ";
        for(int col = 1; col <= 3; col++){
            cout << "|  ";
            pair<int,int> pos = {fila, col};
            if(tablero.find(pos) != tablero.end()){
                cout << tablero.at(pos);
            } else {
                cout << " ";
            }
            cout << "  ";
        }
        cout << "|" << endl;
        if(fila < 3){
            cout << "   |-----|-----|-----|" << endl;
        }
    }
    cout << "   __________________" << endl;
    cout << "\n";
}

bool verificarVictoria(const map<pair<int,int>, char>& tablero, char jugador){
    // Filas
    for(int fila = 1; fila <= 3; fila++){
        auto it1 = tablero.find({fila,1});
        auto it2 = tablero.find({fila,2});
        auto it3 = tablero.find({fila,3});
        
        if(it1 != tablero.end() && it2 != tablero.end() && it3 != tablero.end()){
            if(it1->second == jugador && it2->second == jugador && it3->second == jugador){
                return true;
            }
        }
    }
    
    // Columnas
    for(int col = 1; col <= 3; col++){
        auto it1 = tablero.find({1,col});
        auto it2 = tablero.find({2,col});
        auto it3 = tablero.find({3,col});
        
        if(it1 != tablero.end() && it2 != tablero.end() && it3 != tablero.end()){
            if(it1->second == jugador && it2->second == jugador && it3->second == jugador){
                return true;
            }
        }
    }
    
    // Diagonal principal
    auto it1 = tablero.find({1,1});
    auto it2 = tablero.find({2,2});
    auto it3 = tablero.find({3,3});
    if(it1 != tablero.end() && it2 != tablero.end() && it3 != tablero.end()){
        if(it1->second == jugador && it2->second == jugador && it3->second == jugador){
            return true;
        }
    }
    
    // Diagonal secundaria
    it1 = tablero.find({1,3});
    it2 = tablero.find({2,2});
    it3 = tablero.find({3,1});
    if(it1 != tablero.end() && it2 != tablero.end() && it3 != tablero.end()){
        if(it1->second == jugador && it2->second == jugador && it3->second == jugador){
            return true;
        }
    }
    
    return false;
}

vector<pair<int,int>> obtenerMovimientosDisponibles(const map<pair<int,int>, char>& tablero){
    vector<pair<int,int>> movimientos;
    for(int fila = 1; fila <= 3; fila++){
        for(int col = 1; col <= 3; col++){
            pair<int,int> pos = {fila, col};
            if(tablero.find(pos) == tablero.end()){
                movimientos.push_back(pos);
            }
        }
    }
    return movimientos;
}

// Algoritmo Minimax: La IA explora todos los movimientos posibles recursivamente
int minimax(map<pair<int,int>, char> tablero, bool esMaximizador){
    // Casos base: victoria, derrota o empate
    if(verificarVictoria(tablero, 'O')){
        return 10;  // La IA gana (bueno para la IA)
    }
    if(verificarVictoria(tablero, 'X')){
        return -10; // El humano gana (malo para la IA)
    }
    
    vector<pair<int,int>> movimientos = obtenerMovimientosDisponibles(tablero);
    if(movimientos.empty()){
        return 0;   // Empate
    }
    
    if(esMaximizador){
        // Turno de la IA: busca maximizar el puntaje
        int mejorPuntaje = numeric_limits<int>::min();
        
        for(auto& mov : movimientos){
            tablero[mov] = 'O';
            int puntaje = minimax(tablero, false);
            tablero.erase(mov);
            mejorPuntaje = max(mejorPuntaje, puntaje);
        }
        return mejorPuntaje;
    } else {
        // Turno del humano: la IA asume que juega óptimamente (minimiza)
        int mejorPuntaje = numeric_limits<int>::max();
        
        for(auto& mov : movimientos){
            tablero[mov] = 'X';
            int puntaje = minimax(tablero, true);
            tablero.erase(mov);
            mejorPuntaje = min(mejorPuntaje, puntaje);
        }
        return mejorPuntaje;
    }
}

pair<int,int> obtenerMejorMovimiento(const map<pair<int,int>, char>& tablero){
    pair<int,int> mejorMovimiento = {-1, -1};
    int mejorPuntaje = numeric_limits<int>::min();
    
    vector<pair<int,int>> movimientos = obtenerMovimientosDisponibles(tablero);
    
    cout << "La IA esta pensando..." << endl;
    
    for(auto& mov : movimientos){
        map<pair<int,int>, char> tableroTemporal = tablero;
        tableroTemporal[mov] = 'O';
        
        int puntaje = minimax(tableroTemporal, false);
        
        debugPrint("Evaluando movimiento (" + to_string(mov.first) + "," + 
                   to_string(mov.second) + ") con puntaje: " + to_string(puntaje));
        
        if(puntaje > mejorPuntaje){
            mejorPuntaje = puntaje;
            mejorMovimiento = mov;
        }
    }
    
    return mejorMovimiento;
}

void juegoVsIA(){
    map<pair<int,int>, char> tablero;
    
    cout << "\n=== GATO vs IA ===\n";
    cout << "Tu eres X, la IA es O\n";
    cout << "La IA juega de manera OPTIMA - intenta ganarle!\n\n";
    
    int turno;
    cout << "Quien empieza? (1 = Tu, 2 = IA): ";
    cin >> turno;
    
    while(true){
        system("cls");
        
        // Verificar victoria
        if(verificarVictoria(tablero, 'X')){
            imprimirTablero(tablero);
            cout << "¡FELICIDADES! ¡GANASTE! (Esto es casi imposible...)" << endl;
            return;
        }
        if(verificarVictoria(tablero, 'O')){
            imprimirTablero(tablero);
            cout << "LA IA GANO. Mejor suerte la proxima vez!" << endl;
            return;
        }

        // Verificar empate
        if(tablero.size() == 9){
            imprimirTablero(tablero);
            cout << "EMPATE! Bien jugado!" << endl;
            return;
        }

        imprimirTablero(tablero);
        
        if(turno == 1){
            // Turno del jugador humano
            int x, y;
            cout << "Tu turno (X) - Ingresa fila y columna (1-3): ";
            cin >> x >> y;
            
            // Validar rango
            if(x < 1 || x > 3 || y < 1 || y > 3){
                cout << "Posicion invalida! Debe ser entre 1 y 3." << endl;
                system("pause");
                continue;
            }
            
            pair<int,int> pos = {x, y};
            if(tablero.find(pos) != tablero.end()){ 
                cout << "Esa casilla ya esta ocupada!" << endl;
                system("pause");
                continue;
            }   
            
            tablero[pos] = 'X';
            turno = 2;
            
        } else {
            // Turno de la IA
            cout << "Turno de la IA (O)..." << endl;
            
            pair<int,int> movimiento = obtenerMejorMovimiento(tablero);
            tablero[movimiento] = 'O';
            
            cout << "La IA jugo en (" << movimiento.first << "," << movimiento.second << ")" << endl;
            system("pause");
            
            turno = 1;
        }
    }
}

 
int main() {
    while(true){
        system("cls");
        
        cout << "\n========== JUEGO DE GATO ==========\n";
        cout << "1. Jugar vs IA (DIFICIL)\n";
        cout << "2. Salir\n";
        cout << "===================================\n";
        cout << "Selecciona una opcion: ";
        
        int opcion;
        cin >> opcion;
        
        if(opcion == 1){
            juegoVsIA();
        } else if(opcion == 2){
            cout << "Gracias por jugar!" << endl;
            break;
        } else {
            cout << "Opcion invalida!" << endl;
        }
        
        cout << "\nPresiona enter para continuar...";
        cin.ignore();
        cin.get();
    }
    
    return 0;
}