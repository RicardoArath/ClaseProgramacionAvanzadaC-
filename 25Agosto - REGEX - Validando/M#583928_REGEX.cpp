#include <iostream>
#include <regex>
#include <string>

int main() {
    // Expresión regular para validar un correo electrónico
    std::regex patron_email(R"(^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\.[a-zA-Z0-9-.]+$)");

    std::string email;
    std::cout << "Introduce un correo electronico: ";
    std::getline(std::cin, email);

    // Verificamos si el correo cumple con el patrón
    if (std::regex_match(email, patron_email)) {
        std::cout << "Es un correo electronico valido." << std::endl;
    } else {
        std::cout << "No es un correo electronico valido." << std::endl;
    }

    return 0;
}
