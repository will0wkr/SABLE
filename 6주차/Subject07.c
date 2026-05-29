#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define BLOCK_SIZE 8
#define MAX_SIZE 128

void print_hex(const char *title, uint8_t *data, int len){
	printf("%s: ", title);

	for(int i = 0; i < len; i++){
		printf("%02X ", data[i]);
	}

	printf("\n");
}

void print_text(const char *title, uint8_t *data, int len){
	printf("%s: ", title);

	for(int i = 0; i < len; i++){
		printf("%c", data[i]);
	}

	printf("\n");
}

void print_plain_info(uint8_t *plain, int original_len, int padded_len){
	printf("Original length: %d\n", original_len);
	printf("Padded length  : %d\n\n", padded_len);

	print_text("Plain", plain, original_len);
	print_hex("Plain Hex", plain, padded_len);
}

void print_mode_result(const char *mode, uint8_t *cipher, uint8_t *decrypted, int original_len, int padded_len){
	printf("\n[%s]\n", mode);
	print_hex("Cipher", cipher, padded_len);
	print_text("Decrypted", decrypted, original_len);
}

int get_padded_len(int original_len){
	return ((original_len + BLOCK_SIZE - 1) / BLOCK_SIZE) * BLOCK_SIZE;
}

void encrypt_block(uint8_t *in, uint8_t *out, uint8_t *key){
	for(int i = 0; i < BLOCK_SIZE; i++){
		out[i] = in[i] ^ key[i];
	}
}

void decrypt_block(uint8_t *in, uint8_t *out, uint8_t *key){
	encrypt_block(in, out, key);
}

void xor_block(uint8_t *a, uint8_t *b, uint8_t *out){
	for(int i = 0; i < BLOCK_SIZE; i++){
		out[i] = a[i] ^ b[i];
	}
}

void increase_iv(uint8_t *iv){
	for(int i = BLOCK_SIZE - 1; i >= 0; i--){
		iv[i]++;

		if(iv[i] != 0){
			break;
		}
	}
}

void ecb_encrypt(uint8_t *plain, uint8_t *cipher, int len, uint8_t *key){
	for(int i = 0; i < len; i += BLOCK_SIZE){
		encrypt_block(plain + i, cipher + i, key);
	}
}

void ecb_decrypt(uint8_t *cipher, uint8_t *plain, int len, uint8_t *key){
	for(int i = 0; i < len; i += BLOCK_SIZE){
		decrypt_block(cipher + i, plain + i, key);
	}
}

void cbc_encrypt(uint8_t *plain, uint8_t *cipher, int len, uint8_t *key, uint8_t *iv){
	uint8_t temp[BLOCK_SIZE];

	for(int i = 0; i < len; i += BLOCK_SIZE){
		xor_block(plain + i, iv, temp);
		encrypt_block(temp, cipher + i, key);
		memcpy(iv, cipher + i, BLOCK_SIZE);
	}
}

void cbc_decrypt(uint8_t *cipher, uint8_t *plain, int len, uint8_t *key, uint8_t *iv){
	uint8_t temp[BLOCK_SIZE];

	for(int i = 0; i < len; i += BLOCK_SIZE){
		decrypt_block(cipher + i, temp, key);
		xor_block(temp, iv, plain + i);
		memcpy(iv, cipher + i, BLOCK_SIZE);
	}
}

void ctr_crypt(uint8_t *in, uint8_t *out, int len, uint8_t *key, uint8_t *iv){
	uint8_t stream[BLOCK_SIZE];

	for(int i = 0; i < len; i += BLOCK_SIZE){
		encrypt_block(iv, stream, key);

		for(int j = 0; j < BLOCK_SIZE; j++){
			out[i + j] = in[i + j] ^ stream[j];
		}

		increase_iv(iv);
	}
}

int main(){
	uint8_t key[BLOCK_SIZE] = { 'K', 'E', 'Y', '1', '2', '3', '4', '5' };
	uint8_t iv[BLOCK_SIZE] = { 'I', 'V', '1', '2', '3', '4', '5', '6' };

	char message[] = "Hi My name is will0w";

	int original_len = strlen(message);
	int padded_len = get_padded_len(original_len);

	uint8_t plain[MAX_SIZE] = { 0 };
	uint8_t cipher[MAX_SIZE] = { 0 };
	uint8_t decrypted[MAX_SIZE] = { 0 };

	memcpy(plain, message, original_len);

	print_plain_info(plain, original_len, padded_len);

	ecb_encrypt(plain, cipher, padded_len, key);
	ecb_decrypt(cipher, decrypted, padded_len, key);
	print_mode_result("ECB", cipher, decrypted, original_len, padded_len);

	memset(cipher, 0, MAX_SIZE);
	memset(decrypted, 0, MAX_SIZE);

	uint8_t cbc_iv_enc[BLOCK_SIZE];
	uint8_t cbc_iv_dec[BLOCK_SIZE];

	memcpy(cbc_iv_enc, iv, BLOCK_SIZE);
	memcpy(cbc_iv_dec, iv, BLOCK_SIZE);

	cbc_encrypt(plain, cipher, padded_len, key, cbc_iv_enc);
	cbc_decrypt(cipher, decrypted, padded_len, key, cbc_iv_dec);
	print_mode_result("CBC", cipher, decrypted, original_len, padded_len);

	memset(cipher, 0, MAX_SIZE);
	memset(decrypted, 0, MAX_SIZE);

	uint8_t ctr_iv_enc[BLOCK_SIZE];
	uint8_t ctr_iv_dec[BLOCK_SIZE];

	memcpy(ctr_iv_enc, iv, BLOCK_SIZE);
	memcpy(ctr_iv_dec, iv, BLOCK_SIZE);

	ctr_crypt(plain, cipher, padded_len, key, ctr_iv_enc);
	ctr_crypt(cipher, decrypted, padded_len, key, ctr_iv_dec);
	print_mode_result("CTR", cipher, decrypted, original_len, padded_len);

	return 0;
}