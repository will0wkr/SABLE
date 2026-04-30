#include <stdio.h>
#include <string.h>

void clear_input_buffer(){
    int ch;

    while((ch = getchar()) != '\n' && ch != EOF){
    }
}

int main(){

    /**********************************************
        1. 카이사르 암호화
    **********************************************/
    int caesar_key;
    char caesar_message[100];
    char caesar_result[100];

    printf("[ Caesar Cipher ]\n");

    // 입력
    printf("enter caesar key > ");
    scanf("%d", &caesar_key);

    clear_input_buffer();

    printf("enter message > ");
    fgets(caesar_message, sizeof(caesar_message), stdin);

    caesar_message[strcspn(caesar_message, "\n")] = '\0';

    // 암호화
    int caesar_len = strlen(caesar_message);

    caesar_key = caesar_key % 26;

    if(caesar_key < 0){
        caesar_key += 26;
    }

    strcpy(caesar_result, caesar_message);

    for(int i = 0; i < caesar_len; i++){
        if(caesar_result[i] >= 'a' && caesar_result[i] <= 'z'){
            caesar_result[i] = (caesar_result[i] - 'a' + caesar_key) % 26 + 'a';
        }

        else if(caesar_result[i] >= 'A' && caesar_result[i] <= 'Z'){
            caesar_result[i] = (caesar_result[i] - 'A' + caesar_key) % 26 + 'A';
        }
    }

    // 출력
    printf("\n[ Caesar Cipher Result ]\n");
    printf("original message : %s\n", caesar_message);
    printf("caesar key : %d\n", caesar_key);
    printf("encrypted message : %s\n", caesar_result);

    /**********************************************
        2. 단순 치환 암호화
    **********************************************/

    char substitution_key[100];
    char substitution_message[100];
    char substitution_result[100];
    int used[26] = {0};

    printf("\n[ Substitution Cipher ]\n");

    // 입력
    printf("enter substitution key > ");
    scanf("%99s", substitution_key);

    getchar();

    printf("enter message > ");
    fgets(substitution_message, sizeof(substitution_message), stdin);

    substitution_message[strcspn(substitution_message, "\n")] = '\0';

    // key 검사
    if(strlen(substitution_key) != 26){
        printf("\ninvalid substitution key\n");
        printf("key must be 26 lowercase letters.\n");

        return 1;
    }

    for(int i = 0; i < 26; i++){
        if(substitution_key[i] < 'a' || substitution_key[i] > 'z'){
            printf("\ninvalid substitution key\n");
            printf("key must contain only lowercase letters.\n");

            return 1;
        }

        if(used[substitution_key[i] - 'a'] == 1){
            printf("\ninvalid substitution key\n");
            printf("key must not contain duplicate letters.\n");

            return 1;
        }

        used[substitution_key[i] - 'a'] = 1;
    }

    // 암호화
    int substitution_len = strlen(substitution_message);

    strcpy(substitution_result, substitution_message);

    for(int i = 0; i < substitution_len; i++){
        if(substitution_result[i] >= 'a' && substitution_result[i] <= 'z'){
            substitution_result[i] = substitution_key[substitution_result[i] - 'a'];
        }

        else if(substitution_result[i] >= 'A' && substitution_result[i] <= 'Z'){
            substitution_result[i] = substitution_key[substitution_result[i] - 'A'] - 'a' + 'A';
        }
    }

    // 출력
    printf("\n[ Substitution Cipher Result ]\n");
    printf("original message : %s\n", substitution_message);
    printf("plain alphabet : abcdefghijklmnopqrstuvwxyz\n");
    printf("substitution key : %s\n", substitution_key);
    printf("encrypted message : %s\n", substitution_result);

    return 0;
}