#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BLOCK_SIZE 16   // 128bit
#define BLOCK_COUNT 2   // 배열 총 크기
#define TOTAL_SIZE (BLOCK_SIZE * BLOCK_COUNT)

typedef unsigned char byte;

void print_hex(char *name, byte *arr, int size){
    printf("%s: ", name);

    for(int i = 0; i < size; i++){
        printf("%02X ", arr[i]);
    }

    printf("\n");
}

void make_random_key(byte key[BLOCK_COUNT][BLOCK_SIZE]){
    for(int i = 0; i < BLOCK_COUNT; i++){
        for(int j = 0; j < BLOCK_SIZE; j++){
            key[i][j] = rand() % 256;
        }
    }
}

void aes_like_encrypt(byte plain[BLOCK_COUNT][BLOCK_SIZE], byte cipher[BLOCK_COUNT][BLOCK_SIZE], byte key[BLOCK_COUNT][BLOCK_SIZE]){
    for(int i = 0; i < BLOCK_COUNT; i++){
        for(int j = 0; j < BLOCK_SIZE; j++){
            cipher[i][j] = plain[i][j] ^ key[i][j]; // 라운드 키 XOR
        }
    }
}

void aes_like_decrypt(byte cipher[BLOCK_COUNT][BLOCK_SIZE], byte dec[BLOCK_COUNT][BLOCK_SIZE], byte key[BLOCK_COUNT][BLOCK_SIZE]){
    for(int i = 0; i < BLOCK_COUNT; i++){
        for(int j = 0; j < BLOCK_SIZE; j++){
            dec[i][j] = cipher[i][j] ^ key[i][j]; // 같은 키로 복호화
        }
    }
}

void make_keystream(byte *key, byte *nonce, byte *stream){
    for(int i = 0; i < TOTAL_SIZE; i++){
        stream[i] = key[i % 16] + nonce[i % 12]; // key + nonce
    }
}

void xor_crypt(byte *in, byte *out, byte *stream){
    for(int i = 0; i < TOTAL_SIZE; i++){
        out[i] = in[i] ^ stream[i]; // 스트림 XOR
    }
}

int main(){
    srand(time(NULL));

    char text[] = "Hi will0w";
    int len = strlen(text);

    byte aes_plain[BLOCK_COUNT][BLOCK_SIZE] = {0}; // NULL 패딩
    byte aes_key[BLOCK_COUNT][BLOCK_SIZE];
    byte aes_cipher[BLOCK_COUNT][BLOCK_SIZE];
    byte aes_dec[BLOCK_COUNT][BLOCK_SIZE] = {0};

    byte chacha_plain[TOTAL_SIZE] = {0}; // NULL 패딩
    byte chacha_key[16] = "simple_key_12345";
    byte nonce[12] = "nonce_12345";
    byte stream[TOTAL_SIZE];
    byte chacha_cipher[TOTAL_SIZE];
    byte chacha_dec[TOTAL_SIZE];

    memcpy(aes_plain, text, len);
    memcpy(chacha_plain, text, len);

    printf("Plain Text: %s\n\n", text);

    // AES-like
    make_random_key(aes_key);
    aes_like_encrypt(aes_plain, aes_cipher, aes_key);
    aes_like_decrypt(aes_cipher, aes_dec, aes_key);

    print_hex("AES Plain", (byte *)aes_plain, TOTAL_SIZE);
    print_hex("AES Key", (byte *)aes_key, TOTAL_SIZE);
    print_hex("AES Cipher", (byte *)aes_cipher, TOTAL_SIZE);
    printf("AES Decrypt: %s\n\n", (char *)aes_dec);

    // ChaCha20-like
    make_keystream(chacha_key, nonce, stream);
    xor_crypt(chacha_plain, chacha_cipher, stream);
    xor_crypt(chacha_cipher, chacha_dec, stream);

    print_hex("ChaCha Plain", chacha_plain, TOTAL_SIZE);
    print_hex("ChaCha Stream", stream, TOTAL_SIZE);
    print_hex("ChaCha Cipher", chacha_cipher, TOTAL_SIZE);
    printf("ChaCha Decrypt: %s\n", chacha_dec);

    return 0;
}