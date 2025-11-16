//Doy mi palabra que he realizado esta actividad con integridad academica.

#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>

using namespace std;

struct Node {
    int dato;
    Node* izq;
    Node* der;
    
    Node(int valor) : dato(valor), izq(NULL), der(NULL) {}
};

// Función para encontrar la posición de un elemento en el vector inorder
int encontrarPosicion(vector<int>& inorder, int inicio, int fin, int valor) {
    for (int i = inicio; i <= fin; i++) {
        if (inorder[i] == valor) {
            return i;
        }
    }
    return -1;
}

// Función para reconstruir el árbol desde inorder y postorder
Node* construirArbol(vector<int>& inorder, vector<int>& postorder, int inInicio, int inFin, int& postIndex) {
    // Caso base
    if (inInicio > inFin) {
        return NULL;
    }
    
    // El último elemento en postorder es la raíz
    int valorRaiz = postorder[postIndex];
    Node* raiz = new Node(valorRaiz);
    postIndex--;
    
    // Si este nodo no tiene hijos, retornar
    if (inInicio == inFin) {
        return raiz;
    }
    
    // Encontrar la posición de la raíz en inorder
    int posInorder = encontrarPosicion(inorder, inInicio, inFin, valorRaiz);
    
    // Construir subárbol derecho primero (porque vamos de derecha a izquierda en postorder)
    raiz->der = construirArbol(inorder, postorder, posInorder + 1, inFin, postIndex);
    
    // Construir subárbol izquierdo
    raiz->izq = construirArbol(inorder, postorder, inInicio, posInorder - 1, postIndex);
    
    return raiz;
}

// Estructura para almacenar información del mejor nodo terminal
struct MejorNodo {
    int valor;
    int peso;
    
    MejorNodo() : valor(INT_MAX), peso(INT_MAX) {}
};

// Función para encontrar el nodo terminal con el peso mínimo
void encontrarMejorTerminal(Node* nodo, int pesoActual, MejorNodo& mejor) {
    if (nodo == NULL) {
        return;
    }
    
    // Actualizar el peso del camino
    pesoActual += nodo->dato;
    
    // Si es un nodo terminal (hoja)
    if (nodo->izq == NULL && nodo->der == NULL) {
        // Si encontramos un camino con menor peso, o mismo peso pero menor valor
        if (pesoActual < mejor.peso || (pesoActual == mejor.peso && nodo->dato < mejor.valor)) {
            mejor.peso = pesoActual;
            mejor.valor = nodo->dato;
        }
        return;
    }
    
    // Recursión en subárboles
    encontrarMejorTerminal(nodo->izq, pesoActual, mejor);
    encontrarMejorTerminal(nodo->der, pesoActual, mejor);
}

// Función para liberar la memoria del árbol
void liberarArbol(Node* nodo) {
    if (nodo == NULL) {
        return;
    }
    liberarArbol(nodo->izq);
    liberarArbol(nodo->der);
    delete nodo;
}

int main() {
    try {
        int cases;
        cin >> cases;
        
        for (int c = 0; c < cases; c++) {
            int numNodes;
            cin >> numNodes;
            
            // Leer inorder
            vector<int> inorder(numNodes);
            for (int i = 0; i < numNodes; i++) {
                cin >> inorder[i];
            }
            
            // Leer postorder
            vector<int> postorder(numNodes);
            for (int i = 0; i < numNodes; i++) {
                cin >> postorder[i];
            }
            
            // Reconstruir el árbol
            int postIndex = numNodes - 1;
            Node* raiz = construirArbol(inorder, postorder, 0, numNodes - 1, postIndex);
            
            // Encontrar el nodo terminal con el peso mínimo
            MejorNodo mejor;
            encontrarMejorTerminal(raiz, 0, mejor);
            
            // Imprimir resultado
            cout << mejor.valor << endl;
            
            // Liberar memoria
            liberarArbol(raiz);
        }
        
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}