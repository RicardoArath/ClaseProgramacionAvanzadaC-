//Doy mi palabra que he realizado esta actividad con integridad academica.

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <map>
#include <set>

using namespace std;

struct Node {
    int dato;
    Node* izq;
    Node* der;
    bool asignado;
    
    Node() : dato(-1), izq(NULL), der(NULL), asignado(false) {}
};

// Función para insertar un nodo en el árbol siguiendo la ruta
bool insertarNodo(Node* raiz, const string& ruta, int valor) {
    Node* actual = raiz;
    
    // Seguir la ruta creando nodos según sea necesario
    for (char direccion : ruta) {
        if (direccion == 'L') {
            if (actual->izq == NULL) {
                actual->izq = new Node();
            }
            actual = actual->izq;
        } else if (direccion == 'R') {
            if (actual->der == NULL) {
                actual->der = new Node();
            }
            actual = actual->der;
        }
    }
    
    // Verificar si el nodo ya fue asignado
    if (actual->asignado) {
        return false; // Valor duplicado
    }
    
    actual->dato = valor;
    actual->asignado = true;
    return true;
}

// Función para verificar si el árbol está completo (todos los nodos internos tienen valores)
bool verificarCompleto(Node* nodo) {
    if (nodo == NULL) {
        return true;
    }
    
    // Si es un nodo hoja
    if (nodo->izq == NULL && nodo->der == NULL) {
        return nodo->asignado;
    }
    
    // Si es un nodo interno, debe tener valor asignado
    if (!nodo->asignado) {
        return false;
    }
    
    // Si tiene un hijo, debe tener ambos para que el camino sea completo hasta las hojas
    // O verificar que todos los caminos existentes estén completos
    bool izqCompleto = verificarCompleto(nodo->izq);
    bool derCompleto = verificarCompleto(nodo->der);
    
    return izqCompleto && derCompleto;
}

// Función para realizar recorrido breadth-first (BFS)
vector<int> breadthFirstTraversal(Node* raiz) {
    vector<int> resultado;
    if (raiz == NULL || !raiz->asignado) {
        return resultado;
    }
    
    queue<Node*> cola;
    cola.push(raiz);
    
    while (!cola.empty()) {
        Node* actual = cola.front();
        cola.pop();
        
        if (actual != NULL && actual->asignado) {
            resultado.push_back(actual->dato);
            
            if (actual->izq != NULL) {
                cola.push(actual->izq);
            }
            if (actual->der != NULL) {
                cola.push(actual->der);
            }
        }
    }
    
    return resultado;
}

// Función para liberar memoria del árbol
void liberarArbol(Node* nodo) {
    if (nodo == NULL) {
        return;
    }
    liberarArbol(nodo->izq);
    liberarArbol(nodo->der);
    delete nodo;
}

int main() {
    int numBinaryTrees;
    cin >> numBinaryTrees;
    cin.ignore(); // Ignorar el salto de línea después del número
    
    for(int t = 0; t < numBinaryTrees; t++) {
        string linea;
        getline(cin, linea);
        
        Node* raiz = new Node();
        bool esValido = true;
        
        // Procesar la línea completa
        size_t pos = 0;
        while (pos < linea.size()) {
            // Encontrar el siguiente paréntesis de apertura
            size_t inicio = linea.find('(', pos);
            if (inicio == string::npos) {
                break;
            }
            
            // Encontrar el paréntesis de cierre correspondiente
            size_t fin = linea.find(')', inicio);
            if (fin == string::npos) {
                break;
            }
            
            string token = linea.substr(inicio, fin - inicio + 1);
            pos = fin + 1;
            
            // Si es el token de terminación "()"
            if (token == "()") {
                break;
            }
            
            // Parsear el token "(valor,ruta)"
            size_t coma = token.find(',');
            if (coma != string::npos && token.front() == '(' && token.back() == ')') {
                int valor = stoi(token.substr(1, coma - 1));
                string ruta = token.substr(coma + 1, token.size() - coma - 2);
                
                // Insertar el nodo en el árbol
                if (!insertarNodo(raiz, ruta, valor)) {
                    esValido = false; // Nodo duplicado
                }
            }
        }
        
        // Verificar si el árbol está completo y es válido
        if (esValido && verificarCompleto(raiz)) {
            vector<int> recorrido = breadthFirstTraversal(raiz);
            
            // Imprimir el recorrido
            for (size_t i = 0; i < recorrido.size(); i++) {
                if (i > 0) cout << " ";
                cout << recorrido[i];
            }
            cout << endl;
        } else {
            cout << "INCOMPLETE" << endl;
        }
        
        // Liberar memoria
        liberarArbol(raiz);
    }
    
    return 0;
}