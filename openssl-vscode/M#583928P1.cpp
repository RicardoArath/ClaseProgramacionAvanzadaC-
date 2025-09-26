#include <openssl/evp.h>
#include <openssl/rand.h>
#include <iostream>
#include <iomanip>

using namespace std;

int main() {
    // Mensaje a encriptar
    string mensaje = "Hola Mundo";
    cout << "Texto original: " << mensaje << endl;
    
    // Clave e IV simples
    unsigned char key[32] = "clave_secreta_123456789012345";
    unsigned char iv[16] = "vector_inicial1";
    
    // Configurar encriptación AES-256-CBC
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);
    
    // Buffer para texto cifrado
    unsigned char ciphertext[64];
    int len;
    int ciphertext_len;
    
    // Encriptar
    EVP_EncryptUpdate(ctx, ciphertext, &len, (unsigned char*)mensaje.c_str(), mensaje.length());
    ciphertext_len = len;
    EVP_EncryptFinal_ex(ctx, ciphertext + len, &len);
    ciphertext_len += len;
    
    // Mostrar resultado
    cout << "Texto encriptado (hex): ";
    for(int i = 0; i < ciphertext_len; i++) {
        cout << hex << setw(2) << setfill('0') << (int)ciphertext[i];
    }
    cout << endl;
    
    EVP_CIPHER_CTX_free(ctx);
    return 0;
}