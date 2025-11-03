#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

bool debug = false;

void debugPrint(const string& mensaje) {
    if (debug) cout << "[DEBUG] " << mensaje << endl;
}

// Estructura del nodo
struct Nodo {
    int dato;
    Nodo* izq;
    Nodo* der;
    
    Nodo(int valor) : dato(valor), izq(NULL), der(NULL) {}
};

// Clase ABB
class ABB {
private:
    Nodo* raiz;
    
    // ========== 1. OPERACIONES FUNDAMENTALES ==========
    
    // Insercion recursiva
    Nodo* insertarRecursivo(Nodo* nodo, int valor) {
        if (nodo == NULL) {
            debugPrint("Insertando " + to_string(valor));
            return new Nodo(valor);
        }
        
        if (valor < nodo->dato) {
            debugPrint("Yendo a la izquierda desde " + to_string(nodo->dato));
            nodo->izq = insertarRecursivo(nodo->izq, valor);
        } else if (valor > nodo->dato) {
            debugPrint("Yendo a la derecha desde " + to_string(nodo->dato));
            nodo->der = insertarRecursivo(nodo->der, valor);
        } else {
            debugPrint("Valor " + to_string(valor) + " ya existe");
        }
        
        return nodo;
    }
    
    // Busqueda recursiva
    Nodo* buscarRecursivo(Nodo* nodo, int valor) {
        if (nodo == NULL || nodo->dato == valor) {
            return nodo;
        }
        
        if (valor < nodo->dato) {
            debugPrint("Buscando " + to_string(valor) + " en subarbol izquierdo de " + to_string(nodo->dato));
            return buscarRecursivo(nodo->izq, valor);
        } else {
            debugPrint("Buscando " + to_string(valor) + " en subarbol derecho de " + to_string(nodo->dato));
            return buscarRecursivo(nodo->der, valor);
        }
    }
    
    // Encontrar el minimo en un subarbol
    Nodo* encontrarMinimo(Nodo* nodo) {
        while (nodo && nodo->izq != NULL) {
            nodo = nodo->izq;
        }
        return nodo;
    }
    
    // Eliminacion recursiva
    Nodo* eliminarRecursivo(Nodo* nodo, int valor) {
        if (nodo == NULL) { // Valor no encontrado
            debugPrint("Valor " + to_string(valor) + " no encontrado para eliminar");
            return NULL;
        }
        
        if (valor < nodo->dato) {
            nodo->izq = eliminarRecursivo(nodo->izq, valor);
            debugPrint("Yendo a la izquierda desde " + to_string(nodo->dato));
        } else if (valor > nodo->dato) {
            nodo->der = eliminarRecursivo(nodo->der, valor);
            debugPrint("Yendo a la derecha desde " + to_string(nodo->dato));
        } else {
            // Caso 1: Nodo hoja (sin hijos)
            if (nodo->izq == NULL && nodo->der == NULL) {
                debugPrint("Eliminando hoja: " + to_string(nodo->dato));
                delete nodo;
                return NULL;
            }
            // Caso 2: Nodo con un solo hijo
            else if (nodo->izq == NULL) {
                debugPrint("Eliminando nodo con hijo derecho: " + to_string(nodo->dato));
                Nodo* temp = nodo->der;
                delete nodo;
                return temp;
            } else if (nodo->der == NULL) {
                debugPrint("Eliminando nodo con hijo izquierdo: " + to_string(nodo->dato));
                Nodo* temp = nodo->izq;
                delete nodo;
                return temp;
            }
            // Caso 3: Nodo con dos hijos
            else {
                debugPrint("Eliminando nodo con dos hijos: " + to_string(nodo->dato));
                Nodo* sucesor = encontrarMinimo(nodo->der);
                nodo->dato = sucesor->dato;
                nodo->der = eliminarRecursivo(nodo->der, sucesor->dato);
            }
        }
        return nodo;
    }
    
    // ========== 2. RECORRIDOS DEL ARBOL ==========
    
    void preordenRecursivo(Nodo* nodo) {
        if (nodo != NULL) {
            cout << nodo->dato << " ";
            preordenRecursivo(nodo->izq);
            preordenRecursivo(nodo->der);
        }
    }
    
    void inordenRecursivo(Nodo* nodo) {
        if (nodo != NULL) {
            inordenRecursivo(nodo->izq);
            cout << nodo->dato << " ";
            inordenRecursivo(nodo->der);
        }
    }
    
    void postordenRecursivo(Nodo* nodo) {
        if (nodo != NULL) {
            postordenRecursivo(nodo->izq);
            postordenRecursivo(nodo->der);
            cout << nodo->dato << " ";
        }
    }
    
    void amplitudIterativo(Nodo* nodo) {
        if (nodo == NULL) return;
        
        queue<Nodo*> cola;
        cola.push(nodo);
        
        while (!cola.empty()) {
            Nodo* actual = cola.front();
            cola.pop();
            cout << actual->dato << " ";
            
            if (actual->izq != NULL) cola.push(actual->izq);
            if (actual->der != NULL) cola.push(actual->der);
        }
    }
    
    // ========== 3. PROPIEDADES Y METRICAS ==========
    
    int calcularAltura(Nodo* nodo) {
        if (nodo == NULL) return 0;
        
        int alturaIzq = calcularAltura(nodo->izq);
        int alturaDer = calcularAltura(nodo->der);
        
        return 1 + max(alturaIzq, alturaDer);
    }
    
    int contarNodos(Nodo* nodo) {
        if (nodo == NULL) return 0;
        return 1 + contarNodos(nodo->izq) + contarNodos(nodo->der);
    }
    
    int contarHojas(Nodo* nodo) {
        if (nodo == NULL) return 0;
        if (nodo->izq == NULL && nodo->der == NULL) return 1;
        return contarHojas(nodo->izq) + contarHojas(nodo->der);
    }
    
    int contarInternos(Nodo* nodo) {
        if (nodo == NULL || (nodo->izq == NULL && nodo->der == NULL)) return 0;
        return 1 + contarInternos(nodo->izq) + contarInternos(nodo->der);
    }
    
    int encontrarMinimo(Nodo* nodo, bool& encontrado) {
        if (nodo == NULL) {
            encontrado = false;
            return -1;
        }
        
        encontrado = true;
        while (nodo->izq != NULL) {
            nodo = nodo->izq;
        }
        return nodo->dato;
    }
    
    int encontrarMaximo(Nodo* nodo, bool& encontrado) {
        if (nodo == NULL) {
            encontrado = false;
            return -1;
        }
        
        encontrado = true;
        while (nodo->der != NULL) {
            nodo = nodo->der;
        }
        return nodo->dato;
    }
    
    // ========== 4. PROBLEMAS AVANZADOS ==========
    
    bool esABBValidoAux(Nodo* nodo, int minimo, int maximo) {
        if (nodo == NULL) return true;
        
        if (nodo->dato <= minimo || nodo->dato >= maximo) {
            return false;
        }
        
        return esABBValidoAux(nodo->izq, minimo, nodo->dato) &&
               esABBValidoAux(nodo->der, nodo->dato, maximo);
    }
    
    void inordenParaVector(Nodo* nodo, vector<int>& elementos) {
        if (nodo != NULL) {
            inordenParaVector(nodo->izq, elementos);
            elementos.push_back(nodo->dato);
            inordenParaVector(nodo->der, elementos);
        }
    }
    
    Nodo* construirABBBalanceado(vector<int>& elementos, int inicio, int fin) {
        if (inicio > fin) return NULL;
        
        int medio = inicio + (fin - inicio) / 2;
        Nodo* nodo = new Nodo(elementos[medio]);
        
        nodo->izq = construirABBBalanceado(elementos, inicio, medio - 1);
        nodo->der = construirABBBalanceado(elementos, medio + 1, fin);
        
        return nodo;
    }
    
    void liberarMemoria(Nodo* nodo) {
        if (nodo != NULL) {
            liberarMemoria(nodo->izq);
            liberarMemoria(nodo->der);
            delete nodo;
        }
    }
    
public:
    ABB() : raiz(NULL) {}
    
    ~ABB() {
        liberarMemoria(raiz);
    }
    
    // Metodos publicos
    void insertar(int valor) {
        raiz = insertarRecursivo(raiz, valor);
    }
    
    bool buscar(int valor) {
        Nodo* resultado = buscarRecursivo(raiz, valor);
        return resultado != NULL;
    }
    
    Nodo* buscarNodo(int valor) {
        return buscarRecursivo(raiz, valor);
    }
    
    void eliminar(int valor) {
        raiz = eliminarRecursivo(raiz, valor);
    }
    
    void mostrarPreorden() {
        cout << "Preorden: ";
        preordenRecursivo(raiz);
        cout << endl;
    }
    
    void mostrarInorden() {
        cout << "Inorden: ";
        inordenRecursivo(raiz);
        cout << endl;
    }
    
    void mostrarPostorden() {
        cout << "Postorden: ";
        postordenRecursivo(raiz);
        cout << endl;
    }
    
    void mostrarAmplitud() {
        cout << "Por niveles: ";
        amplitudIterativo(raiz);
        cout << endl;
    }
    
    int getAltura() {
        return calcularAltura(raiz);
    }
    
    int getTotalNodos() {
        return contarNodos(raiz);
    }
    
    int getNodosHoja() {
        return contarHojas(raiz);
    }
    
    int getNodosInternos() {
        return contarInternos(raiz);
    }
    
    void mostrarMinMax() {
        bool encontradoMin = false, encontradoMax = false;
        int minimo = encontrarMinimo(raiz, encontradoMin);
        int maximo = encontrarMaximo(raiz, encontradoMax);
        
        if (encontradoMin) {
            cout << "Valor minimo: " << minimo << endl;
        } else {
            cout << "Arbol vacio" << endl;
        }
        
        if (encontradoMax) {
            cout << "Valor maximo: " << maximo << endl;
        }
    }
    
    bool esABBValido() {
        return esABBValidoAux(raiz, INT_MIN, INT_MAX);
    }
    
    void balancear() {
        vector<int> elementos;
        inordenParaVector(raiz, elementos);
        debugPrint("Elementos en orden: ");
        for (int val : elementos) {
            debugPrint(to_string(val));
        }
        
        liberarMemoria(raiz);
        raiz = construirABBBalanceado(elementos, 0, elementos.size() - 1);
        
        debugPrint("Arbol balanceado");
    }
    
    bool estaVacio() {
        return raiz == NULL;
    }
};

void mostrarMenu() {
    cout << "\n========== MENU ABB ==========" << endl;
    cout << "1.  Insertar valor" << endl;
    cout << "2.  Buscar valor" << endl;
    cout << "3.  Eliminar valor" << endl;
    cout << "4.  Mostrar Preorden" << endl;
    cout << "5.  Mostrar Inorden" << endl;
    cout << "6.  Mostrar Postorden" << endl;
    cout << "7.  Mostrar por Niveles" << endl;
    cout << "8.  Mostrar Altura" << endl;
    cout << "9.  Mostrar Total de Nodos" << endl;
    cout << "10. Mostrar Nodos Hoja" << endl;
    cout << "11. Mostrar Nodos Internos" << endl;
    cout << "12. Mostrar Minimo y Maximo" << endl;
    cout << "13. Verificar si es ABB valido" << endl;
    cout << "14. Balancear arbol" << endl;
    cout << "15. Activar/Desactivar Debug" << endl;
    cout << "0.  Salir" << endl;
    cout << "=============================" << endl;
    cout << "Opcion: ";
}

void ejecutarPrograma() {
    ABB arbol;
    int opcion, valor;
    
    while (true) {
        system("cls");
        mostrarMenu();
        cin >> opcion;
        
        try {
            switch (opcion) {
                case 1:
                    cout << "Ingresa el valor a insertar: ";
                    cin >> valor;
                    arbol.insertar(valor);
                    cout << "Valor insertado exitosamente!" << endl;
                    break;
                    
                case 2:
                    cout << "Ingresa el valor a buscar: ";
                    cin >> valor;
                    if (arbol.buscar(valor)) {
                        cout << "El valor " << valor << " SI existe en el arbol" << endl;
                    } else {
                        cout << "El valor " << valor << " NO existe en el arbol" << endl;
                    }
                    break;
                    
                case 3:
                    cout << "Ingresa el valor a eliminar: ";
                    cin >> valor;
                    arbol.eliminar(valor);
                    cout << "Valor eliminado exitosamente!" << endl;
                    break;
                    
                case 4:
                    if (arbol.estaVacio()) {
                        cout << "El arbol esta vacio" << endl;
                    } else {
                        arbol.mostrarPreorden();
                    }
                    break;
                    
                case 5:
                    if (arbol.estaVacio()) {
                        cout << "El arbol esta vacio" << endl;
                    } else {
                        arbol.mostrarInorden();
                    }
                    break;
                    
                case 6:
                    if (arbol.estaVacio()) {
                        cout << "El arbol esta vacio" << endl;
                    } else {
                        arbol.mostrarPostorden();
                    }
                    break;
                    
                case 7:
                    if (arbol.estaVacio()) {
                        cout << "El arbol esta vacio" << endl;
                    } else {
                        arbol.mostrarAmplitud();
                    }
                    break;
                    
                case 8:
                    cout << "Altura del arbol: " << arbol.getAltura() << endl;
                    break;
                    
                case 9:
                    cout << "Total de nodos: " << arbol.getTotalNodos() << endl;
                    break;
                    
                case 10:
                    cout << "Nodos hoja: " << arbol.getNodosHoja() << endl;
                    break;
                    
                case 11:
                    cout << "Nodos internos: " << arbol.getNodosInternos() << endl;
                    break;
                    
                case 12:
                    if (arbol.estaVacio()) {
                        cout << "El arbol esta vacio" << endl;
                    } else {
                        arbol.mostrarMinMax();
                    }
                    break;
                    
                case 13:
                    if (arbol.esABBValido()) {
                        cout << "El arbol SI es un ABB valido" << endl;
                    } else {
                        cout << "El arbol NO es un ABB valido" << endl;
                    }
                    break;
                    
                case 14:
                    if (arbol.estaVacio()) {
                        cout << "El arbol esta vacio, no se puede balancear" << endl;
                    } else {
                        arbol.balancear();
                        cout << "Arbol balanceado exitosamente!" << endl;
                    }
                    break;
                    
                case 15:
                    debug = !debug;
                    cout << "Modo debug: " << (debug ? "ACTIVADO" : "DESACTIVADO") << endl;
                    break;
                    
                case 0:
                    cout << "Saliendo del programa..." << endl;
                    return;
                    
                default:
                    cout << "Opcion invalida!" << endl;
            }
        } catch (const exception& e) {
            cerr << "[ERROR] Excepcion capturada: " << e.what() << endl;
        }
        
        system("pause");
    }
}

int main() {
    system("cls");
    try {
        ejecutarPrograma();
    } catch (const exception& e) {
        cerr << "[ERROR] Excepcion capturada en main: " << e.what() << endl;
    }
    return 0;
}