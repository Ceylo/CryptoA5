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

bool verify_message(TcpSocket& stream, mpz_t secret, CurveRef curve)
{
	bool verified = false;
	
	// message (string)
	// u (mpz_t)
	// v (mpz_t)
	
	return verified;
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
        
        mpz_invert(k,

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
	
	pkt << uString;
	pkt << vString;
	
	stream.send(pkt);
}
