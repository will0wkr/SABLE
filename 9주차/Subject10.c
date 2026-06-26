#include <stdio.h>
#include <string.h>

#define PASSWORD_MOD 1000
#define RSA_N 187
#define PUBLIC_KEY_E 7
#define PRIVATE_KEY_D 23

int simple_hash(const char *str, int mod) {
    int hash = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        hash = (hash * 31 + str[i]) % mod;
    }

    return hash;
}

int mod_pow(int base, int exp, int mod) {
    int result = 1;

    for (int i = 0; i < exp; i++) {
        result = (result * base) % mod;
    }

    return result;
}

int salted_hash(const char *password, const char *salt) {
    char temp[200];

    strcpy(temp, password);
    strcat(temp, salt);

    return simple_hash(temp, PASSWORD_MOD);
}

int main() {
    char input_password[100];

    const char real_password[] = "a1b2c3d4";
    const char salt[] = "77";
    const char document[] = "hi my name is will0w";
    const char changed_document[] = "hi my name is hacker";

    int stored_hash = salted_hash(real_password, salt);

    // 1. 비밀번호 인증
    printf("===== [1] 비밀번호 인증 =====\n");
    printf("비밀번호 입력 : ");
    scanf("%99s", input_password);

    int input_hash = salted_hash(input_password, salt);

    printf("salt 값 : %s\n", salt);
    printf("저장된 비밀번호 해시 : %d\n", stored_hash);
    printf("입력한 비밀번호 해시 : %d\n", input_hash);

    if (stored_hash != input_hash) {
        printf("로그인 실패\n");
        return 0;
    }

    printf("로그인 성공\n\n");

    // 2. 문서 해시 생성
    printf("===== [2] 문서 해시 생성 =====\n");
    printf("문서 내용 : %s\n", document);

    int document_hash = simple_hash(document, RSA_N);

    printf("문서 해시 : %d\n\n", document_hash);

    // 3. 개인키로 전자서명 생성
    printf("===== [3] 개인키로 전자서명 생성 =====\n");
    printf("개인 키 d : %d\n", PRIVATE_KEY_D);

    int signature = mod_pow(document_hash, PRIVATE_KEY_D, RSA_N);

    printf("서명 값 : %d\n\n", signature);

    // 4. 공개키로 전자서명 검증
    printf("===== [4] 공개키로 전자서명 검증 =====\n");
    printf("공개 키 e : %d\n", PUBLIC_KEY_E);

    int verified_hash = mod_pow(signature, PUBLIC_KEY_E, RSA_N);

    printf("서명에서 복원된 해시 : %d\n", verified_hash);
    printf("현재 문서 해시 : %d\n", document_hash);

    if (verified_hash == document_hash) {
        printf("서명 검증 성공\n");
        printf("문서 무결성 확인 + 서명자 인증 가능\n\n");
    } else {
        printf("서명 검증 실패\n\n");
    }

    // 5. 문서 변조 테스트
    printf("===== [5] 문서 변조 테스트 =====\n");
    printf("변조된 문서 내용 : %s\n", changed_document);

    int changed_hash = simple_hash(changed_document, RSA_N);

    printf("변조된 문서 해시 : %d\n");
    printf("서명에서 복원된 해시 : %d\n", verified_hash);

    if (verified_hash == changed_hash) {
        printf("서명 검증 성공\n");
    } else {
        printf("서명 검증 실패\n");
        printf("문서가 변조되었습니다.\n");
    }

    return 0;
}