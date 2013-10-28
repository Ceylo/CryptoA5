//
//  server.cpp
//  CryptoA5
//
//  Created by Baptiste on 28/10/13.
//  Copyright (c) 2013 Yalir. All rights reserved.
//

#include "server.h"
#include "util.h"
#include "cipher.h"
#include "SymCipher.h"
#include "transmission.h"
#include <string>
#include <cassert>
#include <iostream>

using namespace std;

void server()
{
	sf::TcpListener listener;
	
	// bind the listener to a port
	if (listener.listen(DHSTS_PORT) != sf::Socket::Done)
	{
		perror("error when listening");
		return;
	}
	
	// accept a new connection
	sf::TcpSocket socket;
	if (listener.accept(socket) != sf::Socket::Done)
	{
		perror("error when receiving new client");
		return;
	}
    
    string curveData = send_random_curve(socket);
	
	// Load curve
	CurveRef curve = CurveCreateFromData(curveData.c_str());
	mpz_t x, secretDSA;
    
    PointRef myPubDSA = create_key(curve, secretDSA);
    PointRef myPubKey = create_key(curve, x);
    send_key(socket, myPubDSA);
    send_key(socket, myPubKey);
	
    PointRef pubDSA_peer = receive_key(socket);
	PointRef pubKey_peer = receive_key(socket);
    
    PointRef sharedSecret = PointCreateMultiple(pubKey_peer, x, curve);

    string concatenatedExponential = concatenate(myPubKey, pubKey_peer);
    
    mpz_t u, v;
    sign_message(concatenatedExponential.data(), concatenatedExponential.length(), u, v, secretDSA, curve);
    
    // Send u and v
	char *uBuffer = NULL;
	char *vBuffer = NULL;
	
	gmp_asprintf(&uBuffer, "%Zd", u);
	gmp_asprintf(&vBuffer, "%Zd", v);
    
    char concatenateBuffer[sizeof(uBuffer) + sizeof(vBuffer)+ 2];
    
    int i;
    for(i = 0; i < sizeof(uBuffer); ++i)
        concatenateBuffer[i] = uBuffer[i];
    
    concatenateBuffer[sizeof(uBuffer)] = 0;

    for(i = sizeof(uBuffer) + 1; i < sizeof(uBuffer) + sizeof(vBuffer)+ 1; ++i)
        concatenateBuffer[i] = uBuffer[i];
    
    concatenateBuffer[sizeof(uBuffer) + sizeof(vBuffer)+ 1] = 0;
    
    string concatenateString = string(concatenateBuffer);
	free(uBuffer), uBuffer = NULL;
	free(vBuffer), vBuffer = NULL;
    
    //encrypt with AES
    void *bits = pointToKey(sharedSecret);
    SymCipherRef cipher = SymCipherCreateWithKey((const unsigned char *) bits);
    free(bits);
    Uint32 outputLength;
    void *encryptedData = SymCipherEncrypt(cipher, concatenateString.data(), (unsigned int)concatenateString.length(), &outputLength);
    
    Packet pkt;
    pkt << outputLength;
    pkt << encryptedData;
    
    socket.send(pkt);
    free(encryptedData), encryptedData = NULL;
    
    //receive alice signature
    Uint32 encryptedSignatureLength;
	size_t receivedLength;
	pkt >> encryptedSignatureLength;
	
	void *encryptedSignature = malloc(encryptedSignatureLength);
	bzero(encryptedSignature, encryptedSignatureLength);
	socket.receive(encryptedSignature, (size_t)encryptedSignatureLength, receivedLength);
	
	assert(encryptedSignatureLength == receivedLength);
    
    //decrypt alice message with AES
    unsigned int decipheredLength = 0;
	void *signature = SymCipherDecrypt(cipher, encryptedSignature, (unsigned)receivedLength, &decipheredLength);
	free(encryptedSignature), encryptedSignature = NULL;
    
    char *signatureBuffer = (char *)signature;
    string uString = string(signatureBuffer);
    signatureBuffer += uString.size() + 1;
    string vString = string(signatureBuffer);
    
	mpz_init_set_str(u, uString.c_str(), 10);
	mpz_init_set_str(v, vString.c_str(), 10);
    
    //Verify her signature
    bool result = verify_message(signature, decipheredLength, u, v, curve, pubDSA_peer);
    
    if(result)
        cout << "ok" << endl;

	
    mpz_clear(x);
    socket.disconnect();
}