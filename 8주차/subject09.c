#include <stdio.h>

// 최대공약수 계산
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// base^exp mod mod 계산
long long mod_pow(long long base, long long exp, long long mod) {
    long long result = 1;

    while (exp > 0) {
        result = (result * base) % mod;
        exp--;
    }

    return result;
}

// e * d ≡ 1 mod phi 를 만족하는 d 찾기
int find_d(int e, int phi) {
    int d = 1;

    while ((e * d) % phi != 1) {
        d++;
    }

    return d;
}

int main() {
    int p = 17;
    int q = 19;

    int n = p * q;                  // n = 323
    int phi = (p - 1) * (q - 1);    // phi = 288

    int e = 5; // 공개 지수

    if (gcd(e, phi) != 1) {
        printf("e와 phi는 서로소가 아닙니다.\n");
        return 1;
    }

    int d = find_d(e, phi); // 개인 지수

    int M = 'A'; // 원본 메시지, ASCII 65

    long long C = mod_pow(M, e, n); // 암호화
    long long decrypted = mod_pow(C, d, n); // 복호화

    printf("p = %d, q = %d\n", p, q);
    printf("n = %d\n", n);
    printf("phi = %d\n", phi);
    printf("공개키 = (%d, %d)\n", n, e);
    printf("개인키 = (%d, %d)\n", n, d);

    printf("\n원본 메시지 M = %d (%c)\n", M, M);
    printf("암호문 C = %lld\n", C);
    printf("복호화 결과 = %lld (%c)\n", decrypted, (char)decrypted);

    return 0;
}