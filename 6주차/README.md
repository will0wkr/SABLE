> 6주 차 정리 내용

# 블록 단위 암호화
- 데이터를 한 글자씩 암호화하는 것이 아니라, 정해진 크기의 덩어리로 나누어 처리하는 방식.
- 고정 길이 비트열을 같은 길이의 비트열로 변환한다고 설명.
```
AES 암호화
128 bits block 1
128 bits block 2
128 bits block 3
...
```
- 해당 블록들을 어떻게 처리할 것인가는 **운영 모드**가 담당.

# 운영 모드
## ECB(Electronic Codebook) 모드
- 각 평문 블록을 독립 암호화.
- 고정된 암호문 블록을 할당하여 같은 키 아해 항상 같은 암호문 블록으로 암호화함.
```
C1 = AES_K(P1)
C2 = AES_K(P2)
C3 = AES_K(P3)
...
```
- 여기서 P1과 P3가 같은 값이라면 ECB 시행 시 C1 == C3의 결과를 갖게 됨.
## CBC(Cipher Block Chaining) 모드
- 블록들을 체인처럼 연결.
- 이전 암호문과 XOR 이후 암·복호화.
```
C1 = AES_K(P1 XOR IV)
C2 = AES_K(P2 XOR C1)
C3 = AES_K(P3 XOR C2)
...
```
- 첫 번째 평문 블록에는 이전 암호문이 없으니 IV 사용.
- CBC의 IV는 비밀일 필요가 없지만 예측 불가·무결성이 보호되어야 함.
- ECB와 달리 같은 평문 블록이어도 암호문이 다름.
- 암호화는 foward cipher, 복호화는 inverse cipher 연산 수행.
## CTR(Counter) 모드
- 블록 암호를 마치 스트림 암호처럼 쓰는 방식.
- 평문을 직접 넣는 게 아니라 카운터 블록을 AES로 암호화해 키스트림처럼 만들고 그 결과를 평문과 XOR.
``` 
O1 = AES_K(T1)
O2 = AES_K(T2)
O3 = AES_K(T3)
...
```
- T는 카운터 블록, O는 아웃풋.
```
C1 = P1 XOR O1
C2 = p2 XOR O2
C3 = P3 XOR O3
...
```
- CTR에서 중요한 것은 T값이 절대 재사용되면 안됨.
- 이또한 foward cipher로 병렬 수행 가능하고, counter에 대한 foward cipher도 미리 수행 가능.
- 즉, 빠르고 병렬화가 좋지만, nonce·counter 관리가 매우 중요.

# IV(Initialization Vector)의 필요성
- IV란? 초기화 벡터로 첫 번째 블록을 암호화할 때 비어있는 자리를 채워주는 초기 입력값.
- 같은 평문을 같은 키로 암호화하더라도 매번 다른 암호문이 나오게 하기 위해 사용.