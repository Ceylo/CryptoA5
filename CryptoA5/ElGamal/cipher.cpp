//
//  cipher.cpp
//  CryptoA5
//
//  Created by Ceylo on 22/10/2013.
//  Copyright (c) 2013 Yalir. All rights reserved.
//

#include "cipher.h"
#include "util.h"
#include <iostream>

string decrypt_message(TcpSocket& stream, mpz_t secret, CurveRef curve)
{
	Packet pkt;
	stream.receive(pkt);
	
	// Extract c1 and c2
    string peerC1xString, peerC1yString, peerC2String;
    pkt >> peerC1xString;
    pkt >> peerC1yString;
    
    mpz_t peerC1x, peerC1y;
    
    mpz_inits(peerC1x, peerC1y, NULL);
    gmp_sscanf(peerC1xString.c_str(), "%Zd", &peerC1x);
    gmp_sscanf(peerC1yString.c_str(), "%Zd", &peerC1y);
    
    PointRef peerC1 = PointCreateFromGMP(peerC1x, peerC1y);
    mpz_clears(peerC1x, peerC1y, NULL);
    
	// a.c1
    PointRef intermediaire = PointCreateMultiple(peerC1, secret, curve);
	string result;
    
	while (pkt >> peerC2String)
	{
		mpz_t peerC2;
		mpz_init(peerC2);
		gmp_sscanf(peerC2String.c_str(), "%Zd", &peerC2);
		
		char s[2] = {'\0', '\0'};
		mpz_t mpzChar;
		mpz_init(mpzChar);
		
		// m = c2 - (a.c1)x
		mpz_sub(mpzChar, peerC2, intermediaire->x); //message
		s[0] = (char)mpz_get_ui(mpzChar);
		
		result.append(string(s));
		mpz_clear(mpzChar);
	}
	
	return result;
}

void encrypt_message(TcpSocket& stream, const string& msg, PointRef pubKey, CurveRef curve)
{
	mpz_t k;
	secure_rand(k, curve->n);
	PointRef p = curve->g;
	PointRef kp = PointCreateMultiple(p, k, curve);
	
	// Send c1
	char *kpxBuffer = NULL;
	char *kpyBuffer = NULL;
	string kpxString, kpyString;
	Packet pkt;
	
	gmp_asprintf(&kpxBuffer, "%Zd", kp->x);
	gmp_asprintf(&kpyBuffer, "%Zd", kp->y);
	
	kpxString = string(kpxBuffer);
	kpyString = string(kpyBuffer);
	free(kpxBuffer), kpxBuffer = NULL;
	free(kpyBuffer), kpyBuffer = NULL;
	
	pkt << kpxString;
	pkt << kpyString;
	
	PointRef kpub = PointCreateMultiple(pubKey, k, curve);
	
	for (size_t i = 0;i < msg.size();i++)
	{
		mpz_t a;
		mpz_init(a);
		mpz_add_ui(a, kpub->x, msg[i]);
		mpz_mod(a, a, curve->mod);
		
		char *buffer = NULL;
		gmp_asprintf(&buffer, "%Zd", a);
		
		cout << string(buffer) << " ";
		pkt << string(buffer);
		
		free(buffer);
		mpz_clear(a);
	}
	
	cout << endl;
	
	stream.send(pkt);
}
