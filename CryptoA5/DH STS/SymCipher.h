
#ifndef SYMCIPHER_H
#define SYMCIPHER_H

#include <stdlib.h>

typedef struct SymCipher_t *SymCipherRef;

#define SYMCIPHER_KEY_LENGTH	(256 / 8)

/** @brief Create a new symetric cipher handle with the key @a keyData
 *
 * Preconditions:
 * - secretKey must be a valid hexadecimal string
 *
 * @param keyHex the SYMCIPHER_KEY_LENGTH bytes key to use for secret key generation
 * for the symetric cipher, as binary data
 * @return a valid symetric cipher handle
 */
SymCipherRef	SymCipherCreateWithKey(const unsigned char *key);


/** @brief Destroy a symetric cipher handle
 *
 * Preconditions:
 * - aSymCipher must be a valid cipher handle
 *
 * @param aSymCipher the cipher handle to destroy
 */
void			SymCipherDestroy(SymCipherRef aSymCipher);


/** @brief Return the key used to generate the secret as binary data
 *
 * Preconditions:
 * - aSymCipher must be a valid cipher handle
 *
 * @param aSymCipher the cipher handle to get the key from
 * @return the SYMCIPHER_KEY_LENGTH bytes generation key as binary data,
 */
unsigned char *	SymCipherGetKey(SymCipherRef aSymCipher);


/** @brief Encrypt the given data @a data of length @a inputLength
 *
 * Preconditions:
 * - aSymCipher must be a valid cipher handle
 * - data must not be null
 * - inputLength must be greater than 0
 * - outputLength must not be null
 *
 * @param aSymCipher the cipher handle to use for encryption
 * @param data the data to encrypt
 * @param inputLength the length of the data to encrypt, in bytes
 * @param outputLength the length of the returned encrypted data
 * @return the encrypted data, or NULL if an error occured. You're responsible for g_freeing this data
 */
void *			SymCipherEncrypt(SymCipherRef aSymCipher,
								 const void *data,
								 unsigned int inputLength,
								 unsigned int *outputLength);


/** @brief Decrypt the given data @a data of length @a inputLength
 *
 * Preconditions:
 * - aSymCipher must be a valid cipher handle
 * - data must not be null
 * - inputLength must be greater than 0
 * - outputLength must not be null
 *
 * @param aSymCipher the cipher handle to use for decryption
 * @param data the data to decrypt
 * @param inputLength the length of the data to decrypt, in bytes
 * @param outputLength the length of the returned decrypted data
 * @return the encrypted data, or NULL if an error occured. You're responsible for g_freeing this data
 */
void *			SymCipherDecrypt(SymCipherRef aSymCipher,
								 const void *data,
								 unsigned int inputLength,
								 unsigned int *outputLength);

/** @brief Securely fill a @buffer of size @l with random buffer
 *
 * @param buffer the buffer that receive the random number
 * @param l the size of the buffer
 */
void SecureRandFill(unsigned char *buffer, size_t l);

#endif
