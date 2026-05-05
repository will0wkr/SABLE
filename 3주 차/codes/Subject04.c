#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 1000
#define SEGMENT_SIZE 4
#define SEGMENT_COUNT 4

// 거듭제곱 함수
long long power_int(int base, int exp){
    long long result = 1;

    for(int i = 0; i < exp; i++){
        result *= base;
    }

    return result;
}

// 문자 시프트 함수
char shift_char(char ch, int key){
    if(ch >= 'a' && ch <= 'z'){
        return (ch - 'a' + key + 26) % 26 + 'a';
    }

    if(ch >= 'A' && ch <= 'Z'){
        return (ch - 'A' + key + 26) % 26 + 'A';
    }

    return ch;
}

// 카이사르 암호화 함수
void caesar_encrypt(char plain[], char cipher[], int key){
    int len = strlen(plain);

    for(int i = 0; i < len; i++){
        cipher[i] = shift_char(plain[i], key);
    }

    cipher[len] = '\0';
}

// 카이사르 복호화 함수
void caesar_decrypt(char cipher[], char plain[], int key){
    caesar_encrypt(cipher, plain, -key);
}

// 구간별 카이사르 암호화 함수
void segmented_caesar_encrypt(char plain[], char cipher[], int keys[]){
    int len = strlen(plain);

    for(int i = 0; i < len; i++){
        int segment = i / SEGMENT_SIZE;
        int key = keys[segment];

        cipher[i] = shift_char(plain[i], key);
    }

    cipher[len] = '\0';
}

// 구간별 카이사르 복호화 함수
void segmented_caesar_decrypt(char cipher[], char plain[], int keys[]){
    int len = strlen(cipher);

    for(int i = 0; i < len; i++){
        int segment = i / SEGMENT_SIZE;
        int key = keys[segment];

        plain[i] = shift_char(cipher[i], -key);
    }

    plain[len] = '\0';
}

// 기본 카이사르 브루트포스 함수
void brute_force_caesar(char cipher[]){
    char result[MAX_LEN];

    printf("\n[ Basic Caesar Brute Force ]\n");
    printf("key space : 26\n\n");

    // 메인 로직: 가능한 key 0~25를 모두 시도
    for(int key = 0; key < 26; key++){
        caesar_decrypt(cipher, result, key);
        printf("key = %2d -> %s\n", key, result);
    }
}

// 구간별 카이사르 브루트포스 함수
void brute_force_segmented_caesar(char cipher[], char target[]){
    char result[MAX_LEN];
    int keys[SEGMENT_COUNT];
    int printed = 0;
    long long attempt = 0;
    long long key_space = power_int(26, SEGMENT_COUNT);

    printf("\n[ Segmented Caesar Brute Force ]\n");
    printf("segment count : %d\n", SEGMENT_COUNT);
    printf("segment size  : %d letters\n", SEGMENT_SIZE);
    printf("key space     : 26^%d = %lld\n\n", SEGMENT_COUNT, key_space);

    // 메인 로직: 4구간의 key 조합을 모두 시도
    for(int k1 = 0; k1 < 26; k1++){
        for(int k2 = 0; k2 < 26; k2++){
            for(int k3 = 0; k3 < 26; k3++){
                for(int k4 = 0; k4 < 26; k4++){
                    keys[0] = k1;
                    keys[1] = k2;
                    keys[2] = k3;
                    keys[3] = k4;

                    segmented_caesar_decrypt(cipher, result, keys);
                    attempt++;

                    // 너무 많이 출력되므로 처음 10개 시도만 예시로 출력
                    if(printed < 10){
                        printf("try %6lld : keys = [%2d, %2d, %2d, %2d] -> %s\n",
                            attempt, k1, k2, k3, k4, result);
                        printed++;
                    }

                    // 실습 확인용: 원문과 일치하는 경우를 찾음
                    if(strcmp(result, target) == 0){
                        printf("\nfound!\n");
                        printf("try   : %lld / %lld\n", attempt, key_space);
                        printf("keys  : [%d, %d, %d, %d]\n", k1, k2, k3, k4);
                        printf("plain : %s\n", result);
                        return;
                    }
                }
            }
        }
    }

    printf("\nnot found\n");
}

// 문자 빈도 계산 함수
void count_frequency(char text[], int freq[]){
    for(int i = 0; i < 26; i++){
        freq[i] = 0;
    }

    int len = strlen(text);

    for(int i = 0; i < len; i++){
        char ch = tolower(text[i]);

        if(ch >= 'a' && ch <= 'z'){
            freq[ch - 'a']++;
        }
    }
}

// 가장 많이 등장한 문자 찾기 함수
int find_most_frequent_letter(int freq[]){
    int max_index = 0;

    for(int i = 1; i < 26; i++){
        if(freq[i] > freq[max_index]){
            max_index = i;
        }
    }

    return max_index;
}

// 빈도 분석 출력 함수
void print_frequency(char text[]){
    int freq[26];
    int total = 0;

    count_frequency(text, freq);

    for(int i = 0; i < 26; i++){
        total += freq[i];
    }

    printf("\n[ Frequency Analysis ]\n");
    printf("ciphertext : %s\n\n", text);

    // 메인 로직: 암호문에서 각 알파벳 등장 횟수와 비율 출력
    for(int i = 0; i < 26; i++){
        double percent = 0.0;

        if(total > 0){
            percent = (double)freq[i] * 100.0 / total;
        }

        printf("%c : %2d  (%5.2f%%)\n", 'a' + i, freq[i], percent);
    }
}

// 빈도 기반 키 추정 함수
void guess_caesar_key_by_frequency(char cipher[]){
    int freq[26];
    char result[MAX_LEN];

    count_frequency(cipher, freq);

    int most = find_most_frequent_letter(freq);

    // 메인 로직: 가장 많이 나온 암호문 문자가 원래 e였다고 가정
    int guessed_key = (most - ('e' - 'a') + 26) % 26;

    caesar_decrypt(cipher, result, guessed_key);

    printf("\n[ Caesar Key Guess By Frequency ]\n");
    printf("most frequent cipher letter : %c\n", 'a' + most);
    printf("assumption : '%c' was originally 'e'\n", 'a' + most);
    printf("guessed key : %d\n", guessed_key);
    printf("guessed plaintext : %s\n", result);
}

// 메인 함수
int main(){
    char plain[MAX_LEN] = "sonagicrytotrack"; 

    char basic_cipher[MAX_LEN];
    char segmented_cipher[MAX_LEN];

    int basic_key = 7;
    int segment_keys[SEGMENT_COUNT] = {3, 10, 5, 1};

    printf("[ Original Plaintext ]\n");
    printf("plain : %s\n", plain);
    printf("length : %d\n", (int)strlen(plain));

    /*
        1. 키가 하나인 카이사르 암호
    */
    caesar_encrypt(plain, basic_cipher, basic_key);

    printf("\n[ Basic Caesar Cipher ]\n");
    printf("real key : %d\n", basic_key);
    printf("key space : 26\n");
    printf("ciphertext : %s\n", basic_cipher);

    /*
        2. 구간(키)이 4개인 카이사르 암호
        ▶ 키 공간 증가
    */
    segmented_caesar_encrypt(plain, segmented_cipher, segment_keys);

    printf("\n[ Segmented Caesar Cipher ]\n");
    printf("real keys : [%d, %d, %d, %d]\n",
        segment_keys[0], segment_keys[1], segment_keys[2], segment_keys[3]);
    printf("key space : 26^4 = %lld\n", power_int(26, 4));
    printf("ciphertext : %s\n", segmented_cipher);

    /*
        브루트포스 비교

        기본 카이사르:
        - key 후보 26개

        구간별 카이사르:
        - key 후보 26^4개
    */
    brute_force_caesar(basic_cipher);
    brute_force_segmented_caesar(segmented_cipher, plain);

    /*
        빈도 분석 예시

        기본 카이사르 암호문에서 가장 많이 나온 문자를 찾는다.
        이 문자가 영어에서 자주 나오는 e였다고 가정하고 key를 추정한다.
    */
    print_frequency(basic_cipher);
    guess_caesar_key_by_frequency(basic_cipher);

    return 0;
}