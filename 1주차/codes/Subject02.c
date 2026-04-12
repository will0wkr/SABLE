// 시나리오 : 공격자의 관점 → 타인이 입력하는 데이터 훔쳐 보기.
#include <stdio.h>
#include <string.h>

void plainText(char text[]);
void encryptedText(char text[]);
void key(char text[]);

// 평문으로 전송 시 대상의 데이터를 온전히 알 수 있음.
void plainText(char text[]){
    printf("\nstart observing\n");
    printf("text : %s\n", text);
}

// 복호화 (키 사용) : 메시지를 받은 상대 시점.
void key(char text[]){
    char decrypt[100];
    int len;

    printf("\n3. POV : on destination");

    len = strlen(text);

    for(int i = 0; i < len; i++){
        decrypt[i] = text[i] - 1;
    }
    decrypt[len] = '\0';

    printf("text : %s\n", decrypt);
}

// 암호화되어 전송 시 당장 그 내용을 파악할 수 없음.
void encryptedText(char text[]){
    char copied[100];
    int len;

    printf("\nstart observing\n");

    strcpy(copied, text);
    len = strlen(copied);

    // 간이 암호화 : 한 글자씩 밀림.
    for(int i = 0; i < len; i++){
        copied[i] += 1;
    }

    printf("text : %s\n", copied);
    key(copied);
}

int main(){
    char plain[100];
    char encrypted[100];

    printf("1. POV : using a plain text.\n");
    printf("message > ");
    fgets(plain, sizeof(plain), stdin);
    plain[strcspn(plain, "\n")] = '\0';

    plainText(plain);

    printf("\n2. POV : encrypted\n");
    printf("message > ");
    fgets(encrypted, sizeof(encrypted), stdin);
    encrypted[strcspn(encrypted, "\n")] = '\0';

    encryptedText(encrypted);

    return 0;
}