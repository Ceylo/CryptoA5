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
	
	mpz_t u;
    mpz_init(u);
    mpz_mod(u, kp->x, curve->n);
    
    
    
	
	stream.send(pkt);
}
