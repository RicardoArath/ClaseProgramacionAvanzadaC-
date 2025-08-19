#include <iostream>
using namespace std;

int main(){
    int *x;
    int y = 1000000000;
    try { 
        x = new int[y];
        x[0] = 10;
        cout << "Puntero: " << (void *) x <<endl;
        delete[] x;
     }
     catch(std::bad_alloc& e){
        cout << e.what();;
        cout <<" Memoria insuficiente" << endl;
     }
     return 0;
}