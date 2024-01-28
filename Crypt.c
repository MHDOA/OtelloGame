#include <ctype.h>
#include <string.h>

// Key values of a and b and a_inv
const int b = 33;
const int a = 127;
const int a_inv = 127;

void EncryptMessage(char* msg, char* cipher) {
    for (int i = 0; i < strlen(msg); i++) {

        if (!isspace(msg[i])&& msg[i] != '\0') {
            cipher[i] = (char)((((a * (msg[i])) + b) % 128));
        } 
        else {
            cipher[i] = msg[i];
        }
    }
}

void DecryptCipher(char* cipher, char* msg) {
    
    for (int i = 0; i < strlen(cipher); i++) {

        if (!isspace(cipher[i]) && cipher[i] != '\0') {
            msg[i] = (char)(((a_inv * ((cipher[i] - b))) % 128));
        } 
        else {
            msg[i] = cipher[i];
        }
    }
}
