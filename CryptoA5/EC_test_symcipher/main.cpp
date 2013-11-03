
#include "SymCipher.h"
#include "ec_operations.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include <openssl/sha.h>

void sha256(mpz_t h, const void *data, size_t dataLength)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, data, dataLength);
    SHA256_Final(hash, &sha256);
	
	mpz_init(h);
	mpz_import (h, 256/8, 1, sizeof(hash[0]), 1, 0, hash);
}

void * pointToKey(PointRef p)
{
	mpz_t hash;
	size_t exportedBytes = 0;
	void *x = mpz_export(NULL, &exportedBytes, 1, 1, 1, 0, p->x);
	sha256(hash, x, exportedBytes);
	void *buffer = mpz_export(NULL, &exportedBytes, 1, 1, 1, 0, hash);
	assert(exportedBytes == 256 / 8);
	
	free(x);
	mpz_clear(hash);
	
	return buffer;
}

int main()
{
	const char plaintext[] = "coucou les gens";
	unsigned plaintextLength = (unsigned)strlen(plaintext)+1;
	void *encrypted;
	char *decrypted;
	unsigned encryptedLength;
	unsigned decryptedLength;
	
	/** with initialized cipher */
	void *keyGenerationData = pointToKey(PointCreateFromInt(22, 33));
	SymCipherRef iniCipher = SymCipherCreateWithKey((const unsigned char *)keyGenerationData);
	free(keyGenerationData), keyGenerationData = NULL;
	
	assert(iniCipher != NULL);
	
	encrypted = SymCipherEncrypt(iniCipher, plaintext, plaintextLength, &encryptedLength);
	assert(encrypted != NULL);
	assert(encryptedLength > 0);
	
	decrypted = (char *)SymCipherDecrypt(iniCipher, encrypted, encryptedLength, &decryptedLength);
	assert(decrypted != NULL);
	assert(decryptedLength > 0);
	assert(strcmp(decrypted, plaintext) == 0);
	
	SymCipherDestroy(iniCipher);
	
	free(encrypted);
	free(decrypted);
	return 0;
}
