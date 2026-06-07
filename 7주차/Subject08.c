#include <stdio.h>
#include <string.h>

long long mod_pow(long long base, long long exp, long long mod){
    long long result = 1;

    while(exp > 0){
        if(exp % 2 == 1){
            result = (result * base) % mod;
        }

        base = (base * base) % mod;
        exp = exp / 2;
    }

    return result;
}

long long public_key_encrypt(long long message, long long e, long long n){
    return mod_pow(message, e, n);
}

long long private_key_decrypt(long long cipher, long long d, long long n){
    return mod_pow(cipher, d, n);
}

void symmetric_encrypt(char plaintext[], unsigned char ciphertext[], int len, unsigned char key){
    for(int i = 0; i < len; i++){
        ciphertext[i] = plaintext[i] ^ key;
    }
}

void symmetric_decrypt(unsigned char ciphertext[], char decrypted[], int len, unsigned char key){
    for(int i = 0; i < len; i++){
        decrypted[i] = ciphertext[i] ^ key;
    }

    decrypted[len] = '\0';
}

void print_hex(unsigned char data[], int len){
    for(int i = 0; i < len; i++){
        printf("%02X ", data[i]);
    }

    printf("\n");
}

int main(){
    long long n = 3233;
    long long e = 17;
    long long d = 2753;

    unsigned char symmetric_key = 123;

    char plaintext[] = "HELLO PUBLIC KEY";
    int len = strlen(plaintext);

    unsigned char ciphertext[100];
    char decrypted[100];

    printf("[1] 공개키 / 개인키\n");
    printf("public key  = (e: %lld, n: %lld)\n", e, n);
    printf("private key = (d: %lld, n: %lld)\n\n", d, n);

    printf("[2] 원래 대칭키\n");
    printf("symmetric key = %d\n\n", symmetric_key);

    printf("[3] 공개키로 대칭키 암호화 - 키 운반\n");
    long long encrypted_key = public_key_encrypt(symmetric_key, e, n);
    printf("encrypted symmetric key = %lld\n\n", encrypted_key);

    printf("[4] 개인키로 대칭키 복호화\n");
    long long recovered_key = private_key_decrypt(encrypted_key, d, n);
    printf("recovered symmetric key = %lld\n\n", recovered_key);

    printf("[5] 대칭키로 실제 데이터 암호화\n");
    printf("plaintext = %s\n", plaintext);

    symmetric_encrypt(plaintext, ciphertext, len, symmetric_key);

    printf("ciphertext hex = ");
    print_hex(ciphertext, len);

    printf("\n[6] 복구한 대칭키로 실제 데이터 복호화\n");

    symmetric_decrypt(ciphertext, decrypted, len, recovered_key);

    printf("decrypted = %s\n", decrypted);

    return 0;
}