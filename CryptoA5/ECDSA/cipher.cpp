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

void sign_message(TcpSocket& stream, const string& msg, mpz_t secret, PointRef pubKey, CurveRef curve)
{
    do {
        mpz_t k;
        secure_rand(k, curve->n);
        PointRef p = curve->g;
        PointRef kp = PointCreateMultiple(p, k, curve);
        
        mpz_t u;
        mpz_init(u);
        mpz_mod(u, kp->x, curve->n);
        
        mpz_t v, msgHashed;
        mpz_inits(v, msgHashed, NULL);
        
        sha256(msgHashed, msg);
        
        mpz_mul(v, secret, u);
        mpz_mod(v,v,curve->n);
        
        mpz_add(v, msgHashed, v);
        mpz_mod(v,v, curve->n);
        
        mpz_t kinv;
        mpz_init(kinv);
        
        mpz_invert(kinv, k, curve->n);
        
        mpz_mul(v, v, kinv);
        mpz_mod(v, v, curve->n);

    } while ((mpz_cmp_ui(u, 0) == 0) && (mpz_cmp_ui(v, 0) == 0));

    
    // Send u and v
	char *uBuffer = NULL;
	char *vBuffer = NULL;
	string uString, vString;
	Packet pkt;
	
	gmp_asprintf(&uBuffer, "%Zd", u);
	gmp_asprintf(&vBuffer, "%Zd", v);
	
	uString = string(uBuffer);
	vString = string(vBuffer);
	free(uBuffer), uBuffer = NULL;
	free(vBuffer), vBuffer = NULL;
	
    pkt << msg;
	pkt << uString;
	pkt << vString;
	
	stream.send(pkt);
}
