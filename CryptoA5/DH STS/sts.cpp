//
//  sts.cpp
//  CryptoA5
//
//  Created by Ceylo on 02/11/2013.
//  Copyright (c) 2013 Yalir. All rights reserved.
//

#include "sts.h"
#include "transmission.h"
#include "util.h"
#include "SymCipher.h"
#include "dsa.h"
#include <cassert>
#include <iostream>

using namespace std;

void sts_from_curve_maker(STSContext& context)
{
	sts_priv_maker_setup(context);
	sts_priv_diffie_hellman(context);
	sts_priv_show_my_idendity(context);
	sts_priv_verify_peer_identity(context);
}

void sts_from_curve_receiver(STSContext& context)
{
	sts_priv_receiver_setup(context);
	sts_priv_diffie_hellman(context);
	sts_priv_verify_peer_identity(context);
	sts_priv_show_my_idendity(context);
}

void sts_priv_maker_setup(STSContext& context)
{
	TcpListener listener;
	
	// Bind the listener to a port
	if (listener.listen(DHSTS_PORT) != sf::Socket::Done)
	{
		perror("error when listening");
		exit(1);;
	}
	
	// Wait for a connection
	if (listener.accept(context.stream) != sf::Socket::Done)
	{
		perror("error when receiving new client");
		exit(1);
	}
    
	// Send and load curve
    string curveData = send_random_curve(context.stream);
	context.curve = CurveCreateFromData(curveData.c_str());
	assert(context.curve != NULL);
}

void sts_priv_receiver_setup(STSContext& context)
{
	// Connect to the server
	Socket::Status status = context.stream.connect(IpAddress::getLocalAddress(), DHSTS_PORT);
	
	if (status != Socket::Status::Done)
	{
		perror("connection to server failed");
		exit(1);
	}
	
	// Receive and create the curve
	context.curve = receive_curve(context.stream);
	assert(context.curve != NULL);
}

void sts_priv_diffie_hellman(STSContext& context)
{
	// Create our key pairs
	context.myDsaPubKey = create_key(context.curve, context.dsaRand);
	context.myDhPubKey = create_key(context.curve, context.dhRand);
	
	// Send them
	send_key(context.stream, context.myDsaPubKey);
	send_key(context.stream, context.myDhPubKey);
	
	// Receive the peer pub keys (probably sent while we were sending ours)
    context.peerDsaPubKey = receive_key(context.stream);
	context.peerDhPubKey = receive_key(context.stream);
	
	// Generate the shared secret
	context.sharedSecret = PointCreateMultiple(context.peerDhPubKey, context.dhRand, context.curve);
	
	assert(context.peerDsaPubKey != NULL);
	assert(context.peerDhPubKey != NULL);
	assert(context.sharedSecret != NULL);
#warning Don't know how to check dsaRand and dhRand
}

void sts_priv_show_my_idendity(STSContext& context)
{
	// Create a cipher from our shared secret
	void *keyGenerationData = pointToKey(context.sharedSecret);
	SymCipherRef cipher = SymCipherCreateWithKey((const unsigned char *)keyGenerationData);
	free(keyGenerationData), keyGenerationData = NULL;
	
	// Create a signature
	mpz_t u, v;
	string dataToVerify = concatenate(context.myDhPubKey, context.peerDhPubKey);
	sign_message(dataToVerify.data(), dataToVerify.size(), u, v, context.dsaRand, context.curve);
	
	// Create raw data from the signature
	size_t bitsLength = 0;
	void * bits = uvToData(u, v, bitsLength);
	
	// Encrypt that raw data
	Uint32 cipherLength = 0;
	void *encryptedData = SymCipherEncrypt(cipher, bits, (unsigned int)bitsLength, &cipherLength);
	free(bits), bits = NULL;
	SymCipherDestroy(cipher), cipher = NULL;
	
	// Send the encrypted data
	Packet pkt;
	pkt << cipherLength;
	context.stream.send(pkt);
	context.stream.send(encryptedData, cipherLength);
}

void sts_priv_verify_peer_identity(STSContext& context)
{
	// In cipher: signature of concat(g^x, g^y)
	
	// Create a cipher from our shared secret
	void *keyGenerationData = pointToKey(context.sharedSecret);
	SymCipherRef cipher = SymCipherCreateWithKey((const unsigned char *)keyGenerationData);
	free(keyGenerationData), keyGenerationData = NULL;
	
	
	// Get the signature length
	Packet pkt;
	context.stream.receive(pkt);
	
	Uint32 encryptedSignatureLength;
	pkt >> encryptedSignatureLength;
	
	// Get the signature
	void *encryptedSignature = malloc(encryptedSignatureLength);
	memset(encryptedSignature, 0, encryptedSignatureLength);
	size_t offset = 0;
	Socket::Status connectionStatus;
	
	// Read stream until we got the whole signature
	while (offset < encryptedSignatureLength && connectionStatus != Socket::Status::Disconnected && connectionStatus != Socket::Status::Error)
	{
		size_t receivedLength = 0;
		connectionStatus = context.stream.receive((char *)encryptedSignature + offset, (size_t)encryptedSignatureLength - offset, receivedLength);
		
		offset += receivedLength;
	}
	
	assert(encryptedSignatureLength == offset);
	
	// Decrypt signature (u, v)
	unsigned int decipheredLength = 0;
	void *signature = SymCipherDecrypt(cipher, encryptedSignature, encryptedSignatureLength, &decipheredLength);
	free(encryptedSignature), encryptedSignature = NULL;
	
	// Verify signature (u, v)
	mpz_t u, v;
	dataToUV(signature, u, v);
	free(signature), signature = NULL;
	SymCipherDestroy(cipher), cipher = NULL;
	
	string dataToVerify = concatenate(context.peerDhPubKey, context.myDhPubKey);
	bool signatureOk = verify_message(dataToVerify.data(), dataToVerify.size(), u, v, context.curve, context.peerDsaPubKey);
	
	// So?
	cout << "Peer's signature ok? " << signatureOk << endl;
	assert(signatureOk);
}

