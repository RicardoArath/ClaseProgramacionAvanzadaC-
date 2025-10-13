#include <iostream>
#include <vector>
#include <map>
#include <limits>
#include <algorithm>

using namespace std;

const int INF = numeric_limits<int>::max();

bool debug = false;

void debugPrint(const string& mensaje) {
    if (debug) cout << "[DEBUG] " << mensaje << endl;
}

void dijkstra(char inicio, char destino) {
    int num_nodos = 6; // Número de nodos
    map<char, int> nodos = {{'s', 0}, {'b', 1}, {'c', 2}, {'d', 3}, {'e', 4}, {'t', 5}};
    
    // Crear matriz de adyacencia
    vector<vector<int>> grafo(num_nodos, vector<int>(num_nodos, INF));

    // Agregar aristas (grafo no dirigido)
    grafo[nodos['s']][nodos['b']] = 4;
    grafo[nodos['b']][nodos['s']] = 4;
    grafo[nodos['s']][nodos['c']] = 2;
    grafo[nodos['c']][nodos['s']] = 2;
    grafo[nodos['b']][nodos['d']] = 5;
    grafo[nodos['d']][nodos['b']] = 5;
    grafo[nodos['c']][nodos['d']] = 8;
    grafo[nodos['d']][nodos['c']] = 8;
    grafo[nodos['c']][nodos['e']] = 10;
    grafo[nodos['e']][nodos['c']] = 10;
    grafo[nodos['e']][nodos['d']] = 2;
    grafo[nodos['d']][nodos['e']] = 2;
    grafo[nodos['d']][nodos['t']] = 6;
    grafo[nodos['t']][nodos['d']] = 6;
    grafo[nodos['e']][nodos['t']] = 2;
    grafo[nodos['t']][nodos['e']] = 2;
    
    int inicio_idx = nodos[inicio];
    int destino_idx = nodos[destino];

    vector<int> distancia(num_nodos, INF);
    vector<bool> visitado(num_nodos, false);
    vector<int> padre(num_nodos, -1);

    distancia[inicio_idx] = 0;
    
    // Algoritmo de Dijkstra
    for (int indice = 0; indice < num_nodos; indice++) {
        // Encontrar el nodo no visitado con distancia mínima
        int nodo_minimo = -1;
        for (int nodo = 0; nodo < num_nodos; nodo++) {
            if (!visitado[nodo] && distancia[nodo] != INF) {
                if (nodo_minimo == -1 || distancia[nodo] < distancia[nodo_minimo]) {
                    nodo_minimo = nodo;
                }
            }
        }

        if (nodo_minimo == -1) break; // No hay más nodos accesibles

        visitado[nodo_minimo] = true;

        // Relajar aristas
        for (int vecino = 0; vecino < num_nodos; vecino++) {
            if (grafo[nodo_minimo][vecino] != INF && !visitado[vecino]) {
                if (distancia[nodo_minimo] + grafo[nodo_minimo][vecino] < distancia[vecino]) {
                    distancia[vecino] = distancia[nodo_minimo] + grafo[nodo_minimo][vecino];
                    padre[vecino] = nodo_minimo;
                }
            }
        }
    }
    
    // Reconstruir y mostrar el camino
    if (distancia[destino_idx] == INF) {
        cout << "No existe camino entre " << inicio << " y " << destino << endl;
    } else {
        // Reconstruir camino
        vector<char> camino;
        int actual = destino_idx;
        while (actual != -1) {
            // Convertir índice a carácter
            for (auto& nodo_par : nodos) {
                if (nodo_par.second == actual) {
                    camino.push_back(nodo_par.first);
                    break;
                }
            }
            actual = padre[actual];
        }
        
        // Invertir camino
        reverse(camino.begin(), camino.end());
        
        // Mostrar resultado
        cout << "Camino: ";
        for (char c : camino) {
            cout << c << " ";
        }
        cout << endl;
        cout << "Distancia: " << distancia[destino_idx] << endl;
    }
}

int main() {
    try {
        cout << "Algoritmo de Dijkstra - Ingrese el punto de inicio: " << endl;
        char inicio;
        cin >> inicio;
        debugPrint(string("Punto de inicio: ") + inicio);

        cout << "Algoritmo de Dijkstra - Ingrese el punto de destino: " << endl;
        char destino;
        cin >> destino;
        debugPrint(string("Punto de destino: ") + destino);

        dijkstra(inicio, destino);
    } catch (const exception& e) {
        cerr << "Error inesperado: " << e.what() << endl;
        if (debug) {
            cout << "[DEBUG] Excepción atrapada en main." << endl;
        }
    }
    
    return 0;
}