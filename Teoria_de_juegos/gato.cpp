#include <iostream>
#include <map>

using namespace std;

bool debug = false;

void debugPrint(const string& mensaje) {
    if (debug) cout << "[DEBUG] " << mensaje << endl;
}

void imprimirTablero(map<pair<int,int>, char>& tablero){
    cout << "       1      2      3" << endl;
    cout << "   __________________" << endl;
    
    for(int fila = 1; fila <= 3; fila++){
        cout << fila << "  ";
        for(int col = 1; col <= 3; col++){
            cout << "|  ";
            pair<int,int> pos = {fila, col};
            if(tablero.find(pos) != tablero.end()){
                cout << tablero[pos];
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
        debugPrint("Verificando fila " + to_string(fila) + ": (" + to_string(it1->first.first) + "," + to_string(it1->first.second) + ")");
        
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
        debugPrint("Verificando columna " + to_string(col) + ": (" + to_string(it1->first.first) + "," + to_string(it1->first.second) + ")");
        
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
    debugPrint("Verificando diagonal principal: (" + to_string(it1->first.first) + "," + to_string(it1->first.second) + ")");
    if(it1 != tablero.end() && it2 != tablero.end() && it3 != tablero.end()){
        if(it1->second == jugador && it2->second == jugador && it3->second == jugador){
            return true;
        }
    }
    
    // Diagonal secundaria
    it1 = tablero.find({1,3});
    it2 = tablero.find({2,2});
    it3 = tablero.find({3,1});
    debugPrint("Verificando diagonal secundaria: (" + to_string(it1->first.first) + "," + to_string(it1->first.second) + ")");
    if(it1 != tablero.end() && it2 != tablero.end() && it3 != tablero.end()){
        if(it1->second == jugador && it2->second == jugador && it3->second == jugador){
            return true;
        }
    }
    
    return false;
}

void gato(){
    int x, y;
    map<pair<int,int>, char> tablero;
    int turno = 1;
    
    while(true){
        system("cls");
        
        // Verificar victoria (sin el for loop!)
        if(verificarVictoria(tablero, 'X')){
            imprimirTablero(tablero);
            cout << "GANO EL JUGADOR 1 (X)!" << endl;
            return;
        }
        if(verificarVictoria(tablero, 'O')){
            imprimirTablero(tablero);
            cout << "GANO EL JUGADOR 2 (O)!" << endl;
            return;
        }

        // Verificar empate
        if(tablero.size() == 9){
            imprimirTablero(tablero);
            cout << "EMPATE!" << endl;
            return;
        }

        imprimirTablero(tablero);
        
        if(turno == 1){
            cout << "Jugador 1 (X) - Ingresa fila y columna (1-3): ";
            cin >> x >> y;
            debugPrint("Entrada recibida: (" + to_string(x) + "," + to_string(y) + ")");
            
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
            
            debugPrint("Jugador 1 ingresa posicion: (" + to_string(x) + "," + to_string(y) + ")");
            tablero[pos] = 'X';
            turno = 2;
        } else {
            cout << "Jugador 2 (O) - Ingresa fila y columna (1-3): ";
            cin >> x >> y;
            debugPrint("Entrada recibida: (" + to_string(x) + "," + to_string(y) + ")");
            
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

            debugPrint("Jugador 2 ingresa posicion: (" + to_string(x) + "," + to_string(y) + ")");
            tablero[pos] = 'O';
            turno = 1;
        }
    }
}

int main() {
    system("cls");
    try {
        gato();
    } catch (const exception& e) {
        cerr << "[ERROR] Excepcion capturada en main: " << e.what() << endl;
    }
    return 0;
}