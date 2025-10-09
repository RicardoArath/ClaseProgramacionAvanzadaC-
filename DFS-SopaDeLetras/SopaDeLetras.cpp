#include <iostream>
#include <vector>
#include <string>
using namespace std;

bool debug = false;

void debugPrint(const string& mensaje) {
    if (debug) cout << "[DEBUG] " << mensaje << endl;
}

// Direcciones: N, NE, E, SE, S, SO, O, NO
int direccionFila[] = {-1, -1, 0, 1, 1, 1, 0, -1};  // Cambios en fila
int direccionCol[]  = {0, 1, 1, 1, 0, -1, -1, -1}; // Cambios en columna

// Busca de forma recursiva si la palabra continúa en la misma dirección
bool buscarRecursivo(const vector<vector<char>>& sopa, int fila, int col, const string& palabra, int index, int dir) {
    if (index == palabra.size()) return true;

    int numFilas = sopa.size(); // Número de filas
    int numColumnas = sopa[0].size(); // Número de columnas
    int nuevaFila = fila + direccionFila[dir]; // Nueva fila
    int nuevaCol  = col + direccionCol[dir]; // Nueva columna
    debugPrint("Buscando índice " + to_string(index) + " en (" + to_string(nuevaFila) + "," + to_string(nuevaCol) + ")");

    if (nuevaFila < 0 || nuevaFila >= numFilas || nuevaCol < 0 || nuevaCol >= numColumnas) // Fuera de límites
        return false;

    if (sopa[nuevaFila][nuevaCol] != palabra[index]) // No coincide
        return false;

    return buscarRecursivo(sopa, nuevaFila, nuevaCol, palabra, index + 1, dir);
}

// 🔠 Convierte la palabra encontrada en mayúsculas dentro de la sopa
void marcarMayusculas(vector<vector<char>>& sopa, int fila, int col, 
                      const string& palabra, int dir) {
    for (int k = 0; k < palabra.size(); k++) {
        sopa[fila][col] = toupper(sopa[fila][col]);
        fila += direccionFila[dir];
        col  += direccionCol[dir];
        debugPrint("Marcando mayúscula en (" + to_string(fila) + "," + to_string(col) + ")");
    }
}

// Busca la palabra y la convierte en mayúsculas si la encuentra
bool buscarPalabra(vector<vector<char>>& sopa, const string& palabra) {
    int numFilas = sopa.size(), numColumnas = sopa[0].size();

    for (int fila = 0; fila < numFilas; fila++) {
        for (int col = 0; col < numColumnas; col++) {
            if (sopa[fila][col] == palabra[0]) {
                // Probar las 8 direcciones
                for (int dir = 0; dir < 8; dir++) {
                    if (buscarRecursivo(sopa, fila, col, palabra, 1, dir)) {
                        marcarMayusculas(sopa, fila, col, palabra, dir);
                        cout << "✅ Palabra '" << palabra << "' encontrada en (" 
                             << fila << "," << col << ") dirección " << dir << endl;
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

int main() {
    vector<vector<char>> sopa = {
        {'m','c','u','c','a','r','a','c','h','a','l','a','p','m','i'},
        {'o','f','r','a','i','l','e','c','i','l','l','o','p','o','t'},
        {'s','a','m','a','l','l','i','n','i','h','c','o','c','n','p'},
        {'c','w','o','a','d','u','c','a','r','r','a','b','o','o','o'},
        {'a','s','a','l','t','a','m','o','n','t','e','s','s','c','l'},
        {'o','r','i','n','o','c','e','r','o','n','t','e','n','o','l'},
        {'j','e','d','e','l','e','f','a','n','t','e','i','a','y','o'},
        {'a','t','a','n','a','r','a','j','i','t','r','a','g','a','l'},
        {'b','n','l','r','a','c','o','d','o','r','n','i','z','m','l'},
        {'a','e','l','m','a','m','u','l','o','i','o','r','c','a','i'},
        {'r','i','i','o','e','b','a','t','l','r','a','t','a','c','d'},
        {'a','p','l','r','u','h','i','l','e','c','h','u','z','a','a'},
        {'c','r','o','s','c','n','a','p','a','r','e','n','o','u','m'},
        {'s','e','p','a','r','g','r','c','a','s','t','o','r','g','r'},
        {'e','s','c','o','a','l','l','i','h','c','n','i','h','c','a'}
    };

    try {

    string palabra;
    cout << "Ingrese la palabra a buscar: ";
    cin >> palabra;
    debugPrint("Palabra a buscar: " + palabra);

    if (!buscarPalabra(sopa, palabra)){
        cout << "No se encontró la palabra '" << palabra << "'" << endl;
    } else {
        cout << "\nSopa resultante:\n";
        for (auto& fila : sopa) {
            for (auto& letra : fila) cout << letra << ' ';
            cout << endl;
        }
    }
    } catch (const exception& e) {
        cerr << "Error inesperado: " << e.what() << endl;
        if (debug) {
            cout << "[DEBUG] Excepción atrapada en main." << endl;
        }
    }
    return 0;
}
