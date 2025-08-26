/*PROGRAMA QUE VALIDA CUALQUIER CORREO SIN REGEX*/

#include <iostream>
#include <string>

using namespace std;

bool tieneArroba(string correo){
    //Verifica que tenga un solo arroba
    int numArrobas = 0;
    for (size_t i = 0; i < correo.length(); i++)
    {
        if (correo[i] == '@') { //Si encuentra un arroba 
            numArrobas++;
        }
    }

    if (numArrobas == 1)  return true; // Verifica que solo haya un arroba
        else return false;
}

bool localValido(string correo){
    //Para que el local sea valido necesita: tener al menos un caracter y no puede contener caracteres especiales
    string local =  correo.substr(0, correo.find("@"));
    if(local[0] == '.' || local[local.length() - 1] == '.') return false;  //Si el local empieza o termina con un punto

    for(int i = 0; i < local.length()-1; i++)
        if(local[i] == '.' && local[i + 1] == '.') return false; //No puede tener dos puntos seguidos

    for(int i = 0; i < local.length(); i++){    //Verifica que no contenga caracteres especiales
        if(local[i] == ' ' || local[i] == ',' || local[i] == ';' || local[i] == ':' || local[i] == '/' || local[i] == '\\' || local[i] == '?' || local[i] == '!' || local[i] == '#' || local[i] == '$' || local[i] == '%' || local[i] == '^' || local[i] == '&' || local[i] == '*' || local[i] == '(' || local[i] == ')' || local[i] == '=' || local[i] == '+' || local[i] == '{' || local[i] == '}' || local[i] == '[' || local[i] == ']' || local[i] == '<' || local[i] == '>' || local[i] == '~' || local[i] == '`' || local[i] == '"' || local[i] == '\''){
            return false;
        }
    }

    if (local.length() > 0) return true;  //Si el local tiene al menos un caracter
        else return false;
}

bool hostValido(string correo){
    //Para que un host sea valido necesita: tener al menos un punto y no puede empezar o terminar con un punto
    string host = correo.substr(correo.find("@") + 1);
    if(host[0] == '.' || host[host.length() - 1] == '.') return false;  //Si el host empieza o termina con un punto

    for(int i = 0; i < host.length()-1; i++)
        if(host[i] == '.' && host[i + 1] == '.') return false; //No puede tener dos puntos seguidos

    for(int i = 0; i < host.length(); i++){ //Verifica que no contenga caracteres especiales
        if(host[i] == ' ' || host[i] == ',' || host[i] == ';' || host[i] == ':' || host[i] == '/' || host[i] == '\\' || host[i] == '?' || host[i] == '!' || host[i] == '#' || host[i] == '$' || host[i] == '%' || host[i] == '^' || host[i] == '&' || host[i] == '*' || host[i] == '(' || host[i] == ')' || host[i] == '=' || host[i] == '+' || host[i] == '{' || host[i] == '}' || host[i] == '[' || host[i] == ']' || host[i] == '<' || host[i] == '>' || host[i] == '~' || host[i] == '`' || host[i] == '"' || host[i] == '\''){
            return false;
        }
    }

    //Si el host tiene al menos un punto y no termina con un punto
    if (host.find(".") != string::npos && host.find(".") > 0 && host.find(".") < host.length() - 1) {
        return true;
    } else {
        return false;
    }
}

int main(){
    string correo;

    cout << "Ingrese un correo electronico: ";
    cin >> correo;

    if (correo.find("@") != string::npos && correo.find(".") != string::npos) //Verifica que existan @ y .
        if (tieneArroba(correo) && hostValido(correo) && localValido(correo))
            cout << "El correo electronico es valido." << endl;
        else
            cout << "El correo electronico es invalido." << endl;
    else
        cout << "El correo electronico es invalido." << endl;

    return 0;
}
