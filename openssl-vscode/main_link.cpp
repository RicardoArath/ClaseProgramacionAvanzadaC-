// main_link.cpp
#include <iostream>
#include <openssl/ssl.h>
#include <openssl/err.h>

int main() {
    const SSL_METHOD* method = TLS_method();
    if (!method) { std::cerr << "TLS_method() failed\n"; return 1; }
    SSL_CTX* ctx = SSL_CTX_new(method);
    if (!ctx) { std::cerr << "SSL_CTX_new() failed\n"; ERR_print_errors_fp(stderr); return 1; }
    std::cout << "SSL_CTX creado con éxito\n";
    SSL_CTX_free(ctx);
    return 0;
}
