//
//  cipher.h
//  CryptoA5
//
//  Created by Ceylo on 22/10/2013.
//  Copyright (c) 2013 Yalir. All rights reserved.
//

#ifndef __CryptoA5__cipher__
#define __CryptoA5__cipher__

#include <gmp.h>
#include <string>
#include "ec_operations.h"
#include <SFML/Network.hpp>

using namespace sf;
using namespace std;

/**
 Used to decrypt the message from the peer with our private key
 */
string eg_receive_and_decrypt_message(TcpSocket& stream, mpz_t secret, CurveRef curve);

/**
 Used to encrypt the message to the peer with is public key
 */
void eg_encrypt_and_send_message(TcpSocket& stream, const string& msg, PointRef pubKey, CurveRef curve);

/**
 * Verify the signature of the message from the peer with is public key
 */
bool dsa_receive_and_verify_message(TcpSocket& stream, CurveRef curve, PointRef pubKey);

/**
 * Sign the message and send it to the peer with your private key
 */
void dsa_sign_and_send_message(TcpSocket& stream, const string& msg, mpz_t secret, CurveRef curve);

/**
 * Verify the signature of the message from the peer with is public key
 */
bool dsa_verify_message(const void *data, size_t dataLength, mpz_t u, mpz_t v, CurveRef curve, PointRef pubKey);

/**
 * Sign the message and send it to the peer with your private key
 */
void dsa_sign_message(const void *data, size_t data_length, mpz_t& u, mpz_t& v, mpz_t secret, CurveRef curve);

#endif /* defined(__CryptoA5__cipher__) */
