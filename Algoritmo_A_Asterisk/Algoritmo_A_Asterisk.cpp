#include <iostream>
#include <queue>
#include <cmath>
#include <vector>
#include <set>
#include <iomanip>
#include <algorithm>

using namespace std;

// Estructura para representar un nodo en el grafo
struct Node {
    int x, y;
    float costo;  // Costo desde el nodo inicial
    float heuristica;  // Heuristica (distancia estimada al final)
    float f;  // f = costo + heuristica
    Node* parent; // Puntero al nodo padre para reconstruir la ruta

    Node(int x, int y, float costo = 0, float heuristica = 0, Node* parent = nullptr)
        : x(x), y(y), costo(costo), heuristica(heuristica), f(costo + heuristica), parent(parent) {}
};

// Comparador para la priority_queue
struct CompareNode {
    bool operator()(Node* a, Node* b) const {
        return a->f > b->f;  // Min-heap basado en f
    }
};

class AStarPathfinder {
private:
    int boardSize;
    vector<vector<bool>> obstacles;
    vector<vector<Node*>> allNodes;  // Para rastrear todos los nodos creados
    float heuristicMultiplier;

public:
    AStarPathfinder(int size = 10, float hMult = 1.0)
        : boardSize(size), heuristicMultiplier(hMult) {
        try {
            if (size <= 0 || size > 10) {
                throw runtime_error("ERROR: Tamaño del tablero debe estar entre 1 y 10");
            }
            if (hMult <= 0) {
                throw runtime_error("ERROR: El multiplicador de heuristica debe ser positivo");
            }

            obstacles.assign(size, vector<bool>(size, false));
            allNodes.assign(size, vector<Node*>(size, nullptr));

            cout << "[DEBUG] Tablero de " << size << "x" << size
                 << " Inicializado correctamente" << endl;
            cout << "[DEBUG] Multiplicador de heuristica: " << hMult << endl;
        }
        catch (const exception& e) {
            cerr << e.what() << endl;
            throw;
        }
    }

    ~AStarPathfinder() {
        try {
            // Liberar memoria de todos los nodos creados
            for (int i = 0; i < boardSize; i++) {
                for (int j = 0; j < boardSize; j++) {
                    if (allNodes[i][j] != nullptr) {
                        delete allNodes[i][j];
                        allNodes[i][j] = nullptr;
                    }
                }
            }
            cout << "[DEBUG] Memoria liberada correctamente" << endl;
        }
        catch (const exception& e) {
            cerr << "[ERROR] Error al liberar memoria: " << e.what() << endl;
        }
    }

    // Validar que las coordenadas esten dentro del tablero
    bool isValid(int x, int y) const {
        return x >= 0 && x < boardSize && y >= 0 && y < boardSize;
    }

    // Agregar un obstaculo al tablero
    void addObstacle(int x, int y) {
        try {
            if (!isValid(x, y)) {
                throw runtime_error("ERROR: Coordenadas de obstaculo fuera de rango");
            }
            obstacles[x][y] = true;
            cout << "[DEBUG] Obstaculo agregado en (" << x << ", " << y << ")" << endl;
        }
        catch (const exception& e) {
            cerr << e.what() << endl;
        }
    }

    // Calcular heuristica usando distancia euclidiana
    float calculateHeuristic(int x1, int y1, int x2, int y2) const {
        float dx = x2 - x1; // Diferencia en x
        float dy = y2 - y1; // Diferencia en y
        return heuristicMultiplier * sqrt(dx * dx + dy * dy);
    }

    // Obtener los vecinos validos de una celda (movimiento en 4 direcciones)
    vector<pair<int, int>> getNeighbors(int x, int y) const {
        vector<pair<int, int>> neighbors;
        int dx[] = {0, 0, 1, -1};
        int dy[] = {1, -1, 0, 0};

        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (isValid(nx, ny) && !obstacles[nx][ny]) {
                neighbors.push_back({nx, ny});
            }
        }
        return neighbors;
    }

    // Algoritmo A* principal
    vector<pair<int, int>> findPath(int startX, int startY, int endX, int endY) {
        try {
            cout << "\n[DEBUG] Iniciando busqueda A*" << endl;
            cout << "[DEBUG] Inicio: (" << startX << ", " << startY << ")" << endl;
            cout << "[DEBUG] Final: (" << endX << ", " << endY << ")" << endl;

            if (!isValid(startX, startY) || !isValid(endX, endY)) {
                throw runtime_error("ERROR: Posiciones de inicio o final fuera del tablero");
            }

            if (obstacles[startX][startY] || obstacles[endX][endY]) {
                throw runtime_error("ERROR: Inicio o final estan sobre un obstaculo");
            }

            // Caso especial: inicio = final
            if (startX == endX && startY == endY) {
                cout << "[DEBUG] Inicio y final son el mismo punto" << endl;
                return {{startX, startY}};
            }

            // Cola de prioridad para los nodos abiertos
            priority_queue<Node*, vector<Node*>, CompareNode> openList;
            set<pair<int, int>> closedSet;
            
            // Crear nodo inicial
            float h = calculateHeuristic(startX, startY, endX, endY);
            Node* startNode = new Node(startX, startY, 0, h, nullptr);
            allNodes[startX][startY] = startNode;
            openList.push(startNode);

            int iterations = 0;

            while (!openList.empty()) {
                iterations++;

                // Obtener el nodo con menor f
                Node* current = openList.top();
                openList.pop();

                cout << "[DEBUG] Iteracion " << iterations << ": Explorando ("
                     << current->x << ", " << current->y << ") con f=" << fixed 
                     << setprecision(2) << current->f << " (costo=" << current->costo 
                     << ", heuristica=" << current->heuristica << ")" << endl;

                //  Llegamos al destino?
                if (current->x == endX && current->y == endY) {
                    cout << "[DEBUG] Ruta encontrada en iteracion " << iterations << endl;
                    return reconstructPath(current);
                }

                // Marcar como visitado
                closedSet.insert({current->x, current->y});

                // Explorar vecinos
                vector<pair<int, int>> neighbors = getNeighbors(current->x, current->y);

                for (auto& neighbor : neighbors) {
                    int nx = neighbor.first;
                    int ny = neighbor.second;

                    // Si ya fue visitado, ignorar
                    if (closedSet.count({nx, ny})) {
                        continue;
                    }

                    // Calcular costos
                    float nuevoCosto = current->costo + 1.0f;
                    float nuevoHeuristica = calculateHeuristic(nx, ny, endX, endY);
                    float nuevoF = nuevoCosto + nuevoHeuristica;

                    // Si el nodo no existe o encontramos un mejor camino
                    if (allNodes[nx][ny] == nullptr) {
                        Node* nodoVecino = new Node(nx, ny, nuevoCosto, nuevoHeuristica, current);
                        allNodes[nx][ny] = nodoVecino;
                        openList.push(nodoVecino);

                        cout << "[DEBUG]   Vecino (" << nx << ", " << ny 
                             << ") agregado con f=" << fixed << setprecision(2) 
                             << nuevoF << endl;
                    } else if (nuevoCosto < allNodes[nx][ny]->costo) {
                        // Actualizar el nodo con mejor camino
                        allNodes[nx][ny]->costo = nuevoCosto;
                        allNodes[nx][ny]->heuristica = nuevoHeuristica;
                        allNodes[nx][ny]->f = nuevoF;
                        allNodes[nx][ny]->parent = current;
                        openList.push(allNodes[nx][ny]);

                        cout << "[DEBUG]   Vecino (" << nx << ", " << ny 
                             << ") actualizado con mejor f=" << fixed 
                             << setprecision(2) << nuevoF << endl;
                    }
                }
            }

            cout << "[DEBUG] No se encontro ruta despues de " << iterations 
                 << " iteraciones" << endl;
            throw runtime_error("ERROR: No existe ruta entre los puntos especificados");
        }
        catch (const exception& e) {
            cerr << e.what() << endl;
            return {};
        }
    }

    // Reconstruir la ruta desde el nodo final al inicial
    vector<pair<int, int>> reconstructPath(Node* endNode) {
        vector<pair<int, int>> path;
        try {
            cout << "[DEBUG] Reconstruyendo ruta..." << endl;
            
            Node* current = endNode;
            while (current != nullptr) {
                path.push_back({current->x, current->y});
                cout << "[DEBUG]   <- (" << current->x << ", " << current->y << ")" << endl;
                current = current->parent;
            }
            
            // Invertir el camino para que vaya de inicio a final
            reverse(path.begin(), path.end());
            cout << "[DEBUG] Ruta reconstruida con " << path.size() << " nodos" << endl;
        }
        catch (const exception& e) {
            cerr << "[ERROR] Error en reconstruccion: " << e.what() << endl;
        }
        return path;
    }

    // Imprimir el tablero con la ruta
    void imprimirTablero(const vector<pair<int, int>>& path) {
        try {
            cout << "\n========== TABLERO CON RUTA A* ==========" << endl;
            vector<vector<char>> display(boardSize, vector<char>(boardSize, '.'));

            // Marcar obstaculos
            for (int i = 0; i < boardSize; i++) {
                for (int j = 0; j < boardSize; j++) {
                    if (obstacles[i][j]) {
                        display[i][j] = '#';
                    }
                }
            }

            // Marcar la ruta
            for (size_t i = 0; i < path.size(); i++) {
                int x = path[i].first;
                int y = path[i].second;

                if (i == 0) {
                    display[x][y] = 'E';  // Entrada
                } else if (i == path.size() - 1) {
                    display[x][y] = 'S';  // Salida
                } else {
                    display[x][y] = '*';  // Ruta
                }
            }

            // Imprimir encabezado
            cout << "   ";
            for (int j = 0; j < boardSize; j++) {
                cout << setw(2) << j << " ";
            }
            cout << endl;
            cout << "   ";
            for (int j = 0; j < boardSize; j++) {
                cout << "---";
            }
            cout << endl;

            // Imprimir filas
            for (int i = 0; i < boardSize; i++) {
                cout << setw(2) << i << "|";
                for (int j = 0; j < boardSize; j++) {
                    cout << " " << display[i][j] << " ";
                }
                cout << "|" << endl;
            }

            cout << "   ";
            for (int j = 0; j < boardSize; j++) {
                cout << "---";
            }
            cout << endl;

            cout << "\nLeyenda:" << endl;
            cout << "  E = Punto de inicio" << endl;
            cout << "  S = Punto final" << endl;
            cout << "  * = Ruta encontrada" << endl;
            cout << "  # = Obstaculo" << endl;
            cout << "  . = Espacio libre" << endl;
            cout << "\n  Longitud de ruta: " << path.size() << " pasos" << endl;
            
            // Calcular costo total
            float totalCost = path.size() - 1;
            cout << "  Costo total: " << fixed << setprecision(2) << totalCost << endl;
        }
        catch (const exception& e) {
            cerr << "[ERROR] Error al imprimir tablero: " << e.what() << endl;
        }
    }
};

// Funcion principal
int main() {
    try {
        cout << "==========================================" << endl;
        cout << "    ALGORITMO A* - RUTA MAS CORTA" << endl;
        cout << "==========================================" << endl;

        // Solicitar datos de entrada
        int startX, startY, endX, endY;
        float heuristicMult;
        int numObstacles;

        cout << "\nIngrese el nodo de inicio (x y): ";
        cin >> startX >> startY;

        cout << "Ingrese el nodo final (x y): ";
        cin >> endX >> endY;

        cout << "Ingrese el valor del multiplicador de heuristica (>0): ";
        cin >> heuristicMult;

        cout << " Cuantos obstaculos desea agregar? (maximo 4): ";
        cin >> numObstacles;

        if (numObstacles < 0 || numObstacles > 4) {
            throw runtime_error("ERROR: Numero de obstaculos debe estar entre 0 y 4");
        }

        // Crear el pathfinder
        AStarPathfinder pathfinder(10, heuristicMult);

        // Agregar obstaculos
        if (numObstacles > 0) {
            cout << "\nIngrese las coordenadas de los obstaculos (x y):" << endl;
            for (int i = 0; i < numObstacles; i++) {
                int ox, oy;
                cout << "Obstaculo " << (i + 1) << " (x y): ";
                cin >> ox >> oy;
                pathfinder.addObstacle(ox, oy);
            }
        }

        // Encontrar la ruta
        vector<pair<int, int>> path = pathfinder.findPath(startX, startY, endX, endY);

        if (!path.empty()) {    // Si se encontro una ruta entonces imprimir el tablero
            pathfinder.imprimirTablero(path);
        }

        cout << "\n========== EJECUCION COMPLETADA ==========" << endl;
    }
    catch (const exception& e) {
        cerr << "[FATAL] Error no capturado: " << e.what() << endl;
        return 1;
    }

    return 0;
}