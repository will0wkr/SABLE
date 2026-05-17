#include <stdio.h>
#include <string.h>

#define MAX_SIZE 256

void print_title(const char *title){
	printf("\n========== %s ==========\n", title);
}

void encoding_demo(const char *message){
	print_title("1. Encoding Demo");

	char encoded[MAX_SIZE];
	char decoded[MAX_SIZE];

	int len = strlen(message);

	for(int i = 0; i < len; i++){
		sprintf(encoded + (i * 2), "%02X", (unsigned char)message[i]);
	}

	encoded[len * 2] = '\0';

	for(int i = 0; i < len; i++){
		unsigned int value;
		sscanf(encoded + (i * 2), "%2X", &value);
		decoded[i] = (char)value;
	}

	decoded[len] = '\0';

	printf("Original : %s\n", message);
	printf("Encoded  : %s\n", encoded);
	printf("Decoded  : %s\n", decoded);
	printf("Meaning  : 인코딩은 데이터 표현 형식을 바꾸는 것이다.\n");
	printf("           키 없이 다시 원래 형태로 되돌릴 수 있다.\n");
}

void xor_encrypt(const char *plain, char *cipher, char key){
	int len = strlen(plain);

	for(int i = 0; i < len; i++){
		cipher[i] = plain[i] ^ key;
	}

	cipher[len] = '\0';
}

void xor_decrypt(const char *cipher, char *plain, char key){
	int len = strlen(cipher);

	for(int i = 0; i < len; i++){
		plain[i] = cipher[i] ^ key;
	}

	plain[len] = '\0';
}

void print_hex_data(const char *label, const char *data){
	printf("%s", label);

	for(int i = 0; data[i] != '\0'; i++){
		printf("%02X ", (unsigned char)data[i]);
	}

	printf("\n");
}

void encryption_demo(const char *message){
	print_title("2. Encryption Demo");

	char key = 'K';
	char wrong_key = 'X';

	char cipher[MAX_SIZE];
	char decrypted[MAX_SIZE];
	char wrong_decrypted[MAX_SIZE];

	xor_encrypt(message, cipher, key);
	xor_decrypt(cipher, decrypted, key);
	xor_decrypt(cipher, wrong_decrypted, wrong_key);

	printf("Original        : %s\n", message);
	printf("Key             : %c\n", key);
	print_hex_data("Encrypted(hex)  : ", cipher);
	printf("Decrypted       : %s\n", decrypted);
	printf("Wrong Key Result: %s\n", wrong_decrypted);
	printf("Meaning         : 암호화는 키가 있으면 복호화할 수 있다.\n");
	printf("                  잘못된 키를 쓰면 원문이 제대로 나오지 않는다.\n");
}

unsigned int simple_hash(const char *str){
	unsigned int hash = 5381;

	for(int i = 0; str[i] != '\0'; i++){
		hash = ((hash << 5) + hash) + str[i];
	}

	return hash;
}

void hash_demo(const char *message){
	print_title("3. Hash Demo");

	unsigned int digest1 = simple_hash(message);
	unsigned int digest2 = simple_hash(message);

	printf("Original : %s\n", message);
	printf("Digest   : %u\n", digest1);

	if(digest1 == digest2){
		printf("Verify   : 같은 입력을 다시 해시하면 같은 결과가 나온다.\n");
	}

	printf("Meaning  : 해시는 복호화하지 않는다.\n");
	printf("           원문을 다시 해시해서 기존 해시값과 비교한다.\n");
}

void make_password_hash(const char *password, const char *salt, char *result){
	char combined[MAX_SIZE];

	strcpy(combined, password);
	strcat(combined, salt);

	unsigned int hash = simple_hash(combined);

	sprintf(result, "%u", hash);
}

void password_demo(void){
	print_title("4. Password Verification Demo");

	const char *username = "sangwoo";
	const char *register_password = "myPassword123!";
	const char *salt = "A1B2C3";

	const char *login_password_ok = "myPassword123!";
	const char *login_password_fail = "wrongPassword";

	char stored_hash[MAX_SIZE];
	char input_hash[MAX_SIZE];

	printf("[Register]\n");
	printf("Username       : %s\n", username);
	printf("Plain Password : 입력받지만 저장하지 않음\n");

	make_password_hash(register_password, salt, stored_hash);

	printf("Stored Salt    : %s\n", salt);
	printf("Stored Hash    : %s\n", stored_hash);

	printf("\n[Login Success Case]\n");
	make_password_hash(login_password_ok, salt, input_hash);

	printf("Input Password : %s\n", login_password_ok);
	printf("Input Hash     : %s\n", input_hash);

	if(strcmp(stored_hash, input_hash) == 0){
		printf("Result         : 로그인 성공\n");
	} else {
		printf("Result         : 로그인 실패\n");
	}

	printf("\n[Login Fail Case]\n");
	make_password_hash(login_password_fail, salt, input_hash);

	printf("Input Password : %s\n", login_password_fail);
	printf("Input Hash     : %s\n", input_hash);

	if(strcmp(stored_hash, input_hash) == 0){
		printf("Result         : 로그인 성공\n");
	} else {
		printf("Result         : 로그인 실패\n");
	}

	printf("\nMeaning        : 서버는 비밀번호를 복호화하지 않는다.\n");
	printf("                 입력된 비밀번호에 같은 salt를 붙여 다시 해시하고,\n");
	printf("                 저장된 해시값과 비교한다.\n");
}

void summary(void){
	print_title("5. Summary");

	printf("Encoding   : 표현 형식 변환. 키 없이 되돌릴 수 있음.\n");
	printf("Encryption : 기밀성 보호. 키가 있으면 복호화 가능.\n");
	printf("Hash       : 고정 길이 요약값 생성. 복호화가 아니라 비교에 사용.\n");
	printf("Password   : 원문 비밀번호 저장 X. salt + hash만 저장하고 검증.\n");
}

int main(void){
	const char *message = "Hello Crypto";

	encoding_demo(message);
	encryption_demo(message);
	hash_demo(message);
	password_demo();
	summary();

	return 0;
}