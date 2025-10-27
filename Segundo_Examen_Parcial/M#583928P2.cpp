#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

bool debug = false;

void debugPrint(const string& mensaje) {
    if (debug) cout << "[DEBUG] " << mensaje << endl;
}

// Estructura para almacenar informacion de una serie
struct Serie {
    int suma;
    int filaInicio;
    int colInicio;
    int direccion; // 0: horizontal derecha, 1: vertical abajo
    vector<int> valores;
    
    Serie() : suma(0), filaInicio(-1), colInicio(-1), direccion(-1) {}
};

// Direcciones: 0 = horizontal derecha, 1 = vertical abajo
int direccionFila[] = {0, 1};
int direccionCol[] = {1, 0};
string nombreDir[] = {"Horizontal", "Vertical"};

// DFS recursivo para explorar una serie de 3 numeros
bool explorarSerieRecursiva(const vector<vector<int>>& matriz, int fila, int col, 
                            int profundidad, int dir, vector<int>& valoresActuales) {
    if (profundidad == 3) {
        debugPrint("Serie completa encontrada con suma: " + to_string(valoresActuales[0] + valoresActuales[1] + valoresActuales[2]));
        return true;
    }
    
    int numFilas = matriz.size();
    int numColumnas = matriz[0].size();
    int nuevaFila = fila + direccionFila[dir];
    int nuevaCol = col + direccionCol[dir];
    
    debugPrint("Explorando profundidad " + to_string(profundidad) + " en (" + 
               to_string(nuevaFila) + "," + to_string(nuevaCol) + ")");
    
    // Verificar límites
    if (nuevaFila < 0 || nuevaFila >= numFilas || nuevaCol < 0 || nuevaCol >= numColumnas) {
        debugPrint("Fuera de límites");
        return false;
    }
    
    // Agregar valor actual
    valoresActuales.push_back(matriz[nuevaFila][nuevaCol]);
    
    // Continuar explorando
    return explorarSerieRecursiva(matriz, nuevaFila, nuevaCol, profundidad + 1, dir, valoresActuales);
}

// Busca todas las series posibles de 3 numeros usando DFS
Serie encontrarMayorSerie(const vector<vector<int>>& matriz) {
    int numFilas = matriz.size();
    int numColumnas = matriz[0].size();
    Serie mejorSerie;
    
    debugPrint("Iniciando busqueda de series de 3 numeros...");
    
    try {
        // Explorar cada posicion de la matriz
        for (int fila = 0; fila < numFilas; fila++) {
            for (int col = 0; col < numColumnas; col++) {
                debugPrint("\n=== Explorando posicion (" + to_string(fila) + "," + to_string(col) + ") ===");
                
                // Probar ambas direcciones (horizontal y vertical)
                for (int dir = 0; dir < 2; dir++) {
                    vector<int> valoresActuales;
                    valoresActuales.push_back(matriz[fila][col]);
                    
                    debugPrint("Probando direccion: " + nombreDir[dir]);
                    
                    // Usar DFS para explorar la serie
                    if (explorarSerieRecursiva(matriz, fila, col, 1, dir, valoresActuales)) {
                        // Calcular suma de la serie encontrada
                        int sumaActual = 0;
                        for (int val : valoresActuales) {
                            sumaActual += val;
                        }
                        
                        debugPrint("Suma calculada: " + to_string(sumaActual));
                        
                        // Actualizar mejor serie si es necesario
                        if (sumaActual > mejorSerie.suma) {
                            mejorSerie.suma = sumaActual;
                            mejorSerie.filaInicio = fila;
                            mejorSerie.colInicio = col;
                            mejorSerie.direccion = dir;
                            mejorSerie.valores = valoresActuales;
                            
                            debugPrint("Nueva mejor serie encontrada!");
                        }
                    }
                }
            }
        }
    } catch (const exception& e) {
        cerr << "Error durante la busqueda: " << e.what() << endl;
        if (debug) {
            cout << "[DEBUG] Excepcion capturada en encontrarMayorSerie" << endl;
        }
        throw;
    }
    
    return mejorSerie;
}

// Visualiza la matriz con la serie ganadora marcada
void visualizarResultado(const vector<vector<int>>& matriz, const Serie& serie) {
    vector<vector<int>> matrizVisual = matriz;
    
    cout << "     MATRIZ CON SERIE GANADORA     " << endl;
    
    for (int i = 0; i < matrizVisual.size(); i++) {
        cout << "  ";
        for (int j = 0; j < matrizVisual[i].size(); j++) {
            // Verificar si esta posicion es parte de la serie ganadora
            bool esParte = false;
            for (int k = 0; k < 3; k++) {
                int filaCheck = serie.filaInicio + k * direccionFila[serie.direccion];
                int colCheck = serie.colInicio + k * direccionCol[serie.direccion];
                if (i == filaCheck && j == colCheck) {
                    esParte = true;
                    break;
                }
            }
            
            if (esParte) {
                if(matrizVisual[i][j] < 10)
                    cout << "[\033[1;32m " << matrizVisual[i][j] << "\033[0m]   ";
                if(matrizVisual[i][j] < 100 && matrizVisual[i][j] >=10)
                    cout << "[\033[1;32m" << matrizVisual[i][j] << "\033[0m]  "; // Resaltar en verde
                if(matrizVisual[i][j] >=100)
                    cout << "[\033[1;32m" << matrizVisual[i][j] << "\033[0m] ";
            } else {
                if(matrizVisual[i][j] < 10)
                    cout << " " << matrizVisual[i][j] << "    ";
                if(matrizVisual[i][j] < 100 && matrizVisual[i][j] >=10)
                    cout << " " << matrizVisual[i][j] << "  ";
                if(matrizVisual[i][j] >=100)
                    cout << " " << matrizVisual[i][j] << " ";

            }
        }
        cout << endl;
    }
}

int main() {
    // Matriz predefinida
    vector<vector<int>> matriz = {
        {80, 200, 50, 30, 5},
        {40, 25, 10, 20, 30},
        {67, 110, 20, 200, 100},
        {80, 6, 45, 3, 12},
        {24, 7, 34, 15, 10}
    };
    
    try {
        cout << "BUSQUEDA DE MAYOR SERIE DE 3 NUMEROS" << endl;        
        cout << "Presione ENTER para encontrar la serie de 3 numeros con mayor suma...";
        cin.get();
        debugPrint("\n*** INICIANDO ANALISIS ***\n");
        
        // Encontrar la mejor serie usando DFS
        Serie mejorSerie = encontrarMayorSerie(matriz);
            cout << "         RESULTADO FINAL           " << endl;
            cout << "Serie encontrada: ";
            for (int i = 0; i < mejorSerie.valores.size(); i++) {
                cout << mejorSerie.valores[i];
                if (i < mejorSerie.valores.size() - 1) 
                    cout << " + ";
            }
            cout << " = " << mejorSerie.suma << endl;
            
            cout << "Posicion inicial: (" << mejorSerie.filaInicio 
                 << ", " << mejorSerie.colInicio << ")" << endl;
            cout << "Direccion: " << nombreDir[mejorSerie.direccion] << endl;
            
            visualizarResultado(matriz, mejorSerie);
        
    } catch (const exception& e) {
        cerr << "\nError inesperado: " << e.what() << endl;
        if (debug) {
            cout << "[DEBUG] Excepcion atrapada en main." << endl;
        }
        return 1;
    }
    
    cout << "\nPrograma finalizado exitosamente.\n" << endl;
    return 0;
}