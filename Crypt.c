#include <ctype.h>
#include <string.h>

// Key values of a and b and a_inv
const int b = 33;
const int a = 127;
const int a_inv = 127;

void EncryptMessage(char* msg, char* cipher) {

    for (int i = 0; i < strlen(msg); i++) { // Go char to char

        // Crypt char
        if (!isspace(msg[i])&& msg[i] != '\0') { // Didn't convert white spaces or null
            cipher[i] = (char)((((a * (msg[i])) + b) % 128)); // Crypting
        } 

        // Add white spaces and null
        else {
            cipher[i] = msg[i];
        }
    }
}

void DecryptCipher(char* cipher, char* msg) {
    
    for (int i = 0; i < strlen(cipher); i++) { // Go char to char

        if (!isspace(cipher[i]) && cipher[i] != '\0') { // Didn't convert white spaces or null
            msg[i] = (char)(((a_inv * ((cipher[i] - b))) % 128)); // Encrypting
        } 

        // Add white spaces and null
        else {
            msg[i] = cipher[i]; 
        }
    }
}
