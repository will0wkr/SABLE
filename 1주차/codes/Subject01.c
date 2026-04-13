// 기밀성, 무결성, 인증, 부인 방지 이해하기
// 시나리오 : will0w의 은행 서비스 이용.

#include <stdio.h>
#include <string.h>

int main(){
    int authentic = 0;
    int menu = 0;
    int balance = 500000;
    int transactionCount = 0;

    char newUser[60];
    char newPwd[60];
    char savedUser[] = "will0w";
    char savedPwd[] = "a1b2c3d4";
    char accountNumber[] = "123-456-7890";

    // 로그인 단계 : 사용자 인증
    printf("enter your name. > ");
    scanf("%59s", newUser);

    printf("enter your password. > ");
    scanf("%59s", newPwd);

    // 인증(authentication)
    if(strcmp(newUser, savedUser) == 0 && strcmp(newPwd, savedPwd) == 0){
        authentic = 1;
        printf("\nlogin success.\n");
    }
    else{
        printf("\naccess denied.\n");
    }

    while(authentic == 1){
        printf("\n===== bank menu =====\n");
        printf("1. view account info\n");
        printf("2. deposit money\n");
        printf("3. exit\n");
        printf("> ");
        scanf("%d", &menu);

        if(menu == 1){
            // 기밀성(confidentiality)
            // 인증된 사용자만 계좌 정보 확인 가능
            printf("\n[confidential information]\n");
            printf("user : %s\n", savedUser);
            printf("account number : %s\n", accountNumber);
            printf("balance : %d won\n", balance);
        }
        else if(menu == 2){
            int requestAmount;
            int processedAmount;

            printf("\nhow much will you deposit? > ");
            scanf("%d", &requestAmount);

            // 무결성(integrity)
            // 요청한 금액과 실제 처리 금액이 같은지 확인
            processedAmount = requestAmount;

            // 추후 암호화나 해시를 이용해 전달되고, 
            // 종착지에서 검증하는 역할
            if(requestAmount == processedAmount){
                balance += processedAmount;
                printf("deposit completed.\n");
                printf("requested amount : %d won\n", requestAmount);
                printf("processed amount : %d won\n", processedAmount);
                printf("current balance : %d won\n", balance);

                // 부인방지(non-repudiation)
                // 거래 기록을 남겨 나중에 부인하기 어렵게 함
                transactionCount++;
                printf("\n[transaction record]\n");
                printf("transaction id : %d\n", transactionCount);
                printf("user : %s\n", savedUser);
                printf("type : deposit\n");
                printf("amount : %d won\n", processedAmount);
                printf("status : completed\n");
            }
            else{
                printf("integrity error : amount was changed.\n");
            }
        }
        else if(menu == 3){
            printf("\nservice ended.\n");
            authentic = 0;
        }
        else{
            printf("\nwrong menu.\n");
        }
    }

    return 0;
}
