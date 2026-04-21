#include <stdio.h>
#include <string.h>

int main() {
    int key;
    char message[100];
    char caesar[100];
    char substitution[100];
    char cipher[27];

    printf("enter key > ");
    scanf("%d", &key);
    getchar();   // 엔터 제거

    printf("enter your message > ");
    fgets(message, sizeof(message), stdin);

    // fgets가 넣는 '\n' 제거
    message[strcspn(message, "\n")] = '\0';

    int len = strlen(message);

    // key를 0~25 범위로 맞춤
    key = key % 26;

    // 1. 카이사르 암호
    strcpy(caesar, message);

    for(int i = 0; i < len; i++) {
        if(caesar[i] >= 'a' && caesar[i] <= 'z') {
            caesar[i] = (caesar[i] - 'a' + key) % 26 + 'a';
        }
        else if(caesar[i] >= 'A' && caesar[i] <= 'Z') {
            caesar[i] = (caesar[i] - 'A' + key) % 26 + 'A';
        }
    }

    printf("caesar cipher : %s\n", caesar);

    // 2. 단순 치환 암호용 표 만들기
    for(int i = 0; i < 26; i++) {
        cipher[i] = (i + key) % 26 + 'a';
    }
    cipher[26] = '\0';

    // 3. 단순 치환 암호
    strcpy(substitution, message);

    for(int i = 0; i < len; i++) {
        if(substitution[i] >= 'a' && substitution[i] <= 'z') {
            substitution[i] = cipher[substitution[i] - 'a'];
        }
        else if(substitution[i] >= 'A' && substitution[i] <= 'Z') {
            substitution[i] = cipher[substitution[i] - 'A'] - 'a' + 'A';
        }
    }

    printf("substitution table : %s\n", cipher);
    printf("substitution cipher : %s\n", substitution);

    return 0;
}