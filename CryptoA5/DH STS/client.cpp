//
//  client.cpp
//  CryptoA5
//
//  Created by Baptiste on 28/10/13.
//  Copyright (c) 2013 Yalir. All rights reserved.
//

#include <stdio.h>
#include "util.h"
#include "cipher.h"
#include "transmission.h"
#include "SymCipher.h"
#include <string>
#include <iostream>
#include <cassert>

using namespace std;

CurveRef client_receive_curve(TcpSocket& stream)
{
	Packet pkt;
	stream.receive(pkt);
	
	string curveData;
	pkt >> curveData;
	
	return CurveCreateFromData(curveData.c_str());
}

void client()
{
	TcpSocket socket;
	Socket::Status status = socket.connect(IpAddress::getLocalAddress(), DHSTS_PORT);
	
	if (status != Socket::Status::Done)
	{
		perror("connection to server failed");
		return;
	}
    
    mpz_t dsaRand, dhRand;
    
	CurveRef curve = client_receive_curve(socket);
	
	// Create our key pairs
	PointRef myDsaPubKey = create_key(curve, dsaRand);
	PointRef myDhPubKey = create_key(curve, dhRand);
	
	// Send them
	send_key(socket, myDsaPubKey);
	send_key(socket, myDhPubKey);
	
	// Receive the peer pub keys (probably sent while we were sending ours)
    PointRef peerDsaPubKey = receive_key(socket);
	PointRef peerDhPubKey = receive_key(socket);
    
    PointRef k = PointCreateMultiple(peerDhPubKey, dhRand, curve);
    void *bits = pointToKey(k);
	SymCipherRef cipher = SymCipherCreateWithKey((const unsigned char *)bits);
	free(bits);
	
	// IN cipher:
	// signature of concat(g^x, g^y)
	Packet pkt;
	Uint32 encryptedSignatureLength;
	size_t receivedLength;
	pkt >> encryptedSignatureLength;
	
	// Get signature
	void *encryptedSignature = malloc(encryptedSignatureLength);
	bzero(encryptedSignature, encryptedSignatureLength);
	socket.receive(encryptedSignature, (size_t)encryptedSignatureLength, receivedLength);
	
	assert(encryptedSignatureLength == receivedLength);
	
	// Decrypt signature (u,v)
	unsigned int decipheredLength = 0;
	void *signature = SymCipherDecrypt(cipher, encryptedSignature, (unsigned)receivedLength, &decipheredLength);
	free(encryptedSignature), encryptedSignature = NULL;
	
	// verify signature (u, v)
	string u = string((char *)signature);
	string v = string((char *)signature + u.size() + 1);
	mpz_t uz, vz;
	mpz_init_set_str(uz, u.c_str(), 10);
	mpz_init_set_str(vz, v.c_str(), 10);
	
	string dataToVerify = concatenate(myDhPubKey, peerDhPubKey);
	bool signatureOk = verify_message(dataToVerify.data(), dataToVerify.size(), uz, vz, curve, peerDsaPubKey);
	
	cout << "Peer's signature ok? " << signatureOk << endl;
	
	if (signatureOk)
	{
		// Send our signature
		mpz_t myU, myV;
		sign_message(dataToVerify.data(), dataToVerify.size(), myU, myV, dsaRand, curve);
		
		size_t bitsLength = 0;
		void * bits = uvToData(myU, myV, bitsLength);
		
		Uint32 cipherLength = 0;
		void *encryptedData = SymCipherEncrypt(cipher, bits, (unsigned int)bitsLength, &cipherLength);
		free(bits), bits = NULL;
		
		pkt.clear();
		pkt << cipherLength;
		socket.send(pkt);
		socket.send(encryptedData, cipherLength);
	}
	
    mpz_clear(dsaRand);
	mpz_clear(dhRand);
    socket.disconnect();
}