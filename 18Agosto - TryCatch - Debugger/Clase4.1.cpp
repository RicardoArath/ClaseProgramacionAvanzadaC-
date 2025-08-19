#include <iostream>
using namespace std;

int main(){
    try(){
        throw 'x '; //
    }
    catch(int c){
        cout << "El valor de c es: " << c << endl;
    }
    catch(...) {
        cout << "Excepcion imprevista" << endl;
    }
    return 0;
}