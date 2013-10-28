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
	
	// Decrypt signature
	unsigned int decipheredLength = 0;
	void *signature = SymCipherDecrypt(cipher, encryptedSignature, (unsigned)receivedLength, &decipheredLength);
	free(encryptedSignature), encryptedSignature = NULL;
	
	// verify signature
	
    mpz_clear(dsaRand);
	mpz_clear(dhRand);
    socket.disconnect();
}