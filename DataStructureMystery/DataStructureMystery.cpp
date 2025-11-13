#include <iostream>
#include <vector>
#include <queue>
#include <stack>

using namespace std;

// Función para verificar si la estructura se comporta como una PILA (LIFO)
bool esStack(vector<pair<char, int>>& operaciones){
    stack<int> pila;
    
    for(int i = 0; i < operaciones.size(); i++){
        char operacion = operaciones[i].first;
        int valor = operaciones[i].second;
        
        if(operacion == 'i'){
            // Operación insert: agregar elemento a la pila
            pila.push(valor);
        }
        else if(operacion == 'r'){
            // Operación remove: verificar si el elemento extraído coincide
            // Si la pila está vacía, no puede ser una pila
            if(pila.empty()) return false;
            
            // En una pila, el último en entrar es el primero en salir (LIFO)
            int tope = pila.top();
            pila.pop();
            
            // Si el elemento extraído no coincide, no es una pila
            if(tope != valor) return false;
        }
    }
    
    return true;
}

// Función para verificar si la estructura se comporta como una COLA (FIFO)
bool esQueue(vector<pair<char, int>>& operaciones){
    queue<int> cola;
    
    for(int i = 0; i < operaciones.size(); i++){
        char operacion = operaciones[i].first;
        int valor = operaciones[i].second;
        
        if(operacion == 'i'){
            // Operación insert: agregar elemento a la cola
            cola.push(valor);
        }
        else if(operacion == 'r'){
            // Operación remove: verificar si el elemento extraído coincide
            // Si la cola está vacía, no puede ser una cola
            if(cola.empty()) return false;
            
            // En una cola, el primero en entrar es el primero en salir (FIFO)
            int frente = cola.front();
            cola.pop();
            
            // Si el elemento extraído no coincide, no es una cola
            if(frente != valor) return false;
        }
    }
    
    return true;
}

// Función para verificar si la estructura se comporta como una COLA DE PRIORIDAD
bool esPriorityQueue(vector<pair<char, int>>& operaciones){
    priority_queue<int> colaPrioridad; // Por defecto es max-heap (mayor primero)
    
    for(int i = 0; i < operaciones.size(); i++){
        char operacion = operaciones[i].first;
        int valor = operaciones[i].second;
        
        if(operacion == 'i'){
            // Operación insert: agregar elemento a la cola de prioridad
            colaPrioridad.push(valor);
        }
        else if(operacion == 'r'){
            // Operación remove: verificar si el elemento extraído coincide
            // Si la cola está vacía, no puede ser una cola de prioridad
            if(colaPrioridad.empty()) return false;
            
            // En una cola de prioridad, siempre se extrae el elemento mayor
            int maximo = colaPrioridad.top();
            colaPrioridad.pop();
            
            // Si el elemento extraído no coincide, no es una cola de prioridad
            if(maximo != valor) return false;
        }
    }
    
    return true;
}

int main(){
    int n;

    // Procesar múltiples casos de prueba hasta que n = 0
    while (cin >> n && n != 0){
        vector<pair<char, int>> operaciones; // (operacion, valor)
        
        // Leer todas las operaciones del caso de prueba
        for (int i = 0; i < n; i++){
            char op;
            int val;
            cin >> op >> val;
            // Guardar la operación como 'i' o 'r' (NO convertir a número)
            operaciones.push_back({op, val});
        }
        
        // Verificar cuántas estructuras son posibles
        int posibilidades = 0;
        if (esStack(operaciones)) posibilidades++;
        if (esQueue(operaciones)) posibilidades++;
        if (esPriorityQueue(operaciones)) posibilidades++;

        // Determinar la salida según el número de posibilidades
        if (posibilidades == 0) {
            // Ninguna estructura coincide
            cout << "impossible" << endl;
        }
        else if (posibilidades == 1) {
            // Solo una estructura coincide
            if(esStack(operaciones)) cout << "stack" << endl;
            else if(esQueue(operaciones)) cout << "queue" << endl;
            else cout << "priority queue" << endl;
        }
        else {
            // Más de una estructura coincide
            cout << "uncertain" << endl;
        }
    }
    
    return 0;
}
