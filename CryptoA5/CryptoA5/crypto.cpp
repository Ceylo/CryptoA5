//
//  cipher.cpp
//  CryptoA5
//
//  Created by Ceylo on 22/10/2013.
//  Copyright (c) 2013 Yalir. All rights reserved.
//

#include "crypto.h"
#include "util.h"
#include <iostream>

string eg_receive_and_decrypt_message(TcpSocket& stream, mpz_t secret, CurveRef curve)
{
	Packet pkt;
	stream.receive(pkt);
	
	// Extract c1 and c2
    string peerC1xString, peerC1yString, peerC2String;
    pkt >> peerC1xString;
    pkt >> peerC1yString;
    
    mpz_t peerC1x, peerC1y;
    
	mpz_init_set_str(peerC1x, peerC1xString.c_str(), 10);
	mpz_init_set_str(peerC1y, peerC1yString.c_str(), 10);
    
    PointRef peerC1 = PointCreateFromGMP(peerC1x, peerC1y);
    mpz_clears(peerC1x, peerC1y, NULL);
    
	// a.c1
    PointRef intermediaire = PointCreateMultiple(peerC1, secret, curve);
	string result;
    
	while (pkt >> peerC2String)
	{
		mpz_t peerC2;
		mpz_init_set_str(peerC2, peerC2String.c_str(), 10);
		
		char s[2] = {'\0', '\0'};
		mpz_t mpzChar;
		mpz_init(mpzChar);
		
		// m = c2 - (a.c1)x
		mpz_sub(mpzChar, peerC2, intermediaire->x); //message
		s[0] = (char)mpz_get_ui(mpzChar);
		
		result.append(string(s));
		mpz_clear(mpzChar);
	}
	
    PointDestroy(peerC1);
    PointDestroy(intermediaire);
    
	return result;
}

void eg_encrypt_and_send_message(TcpSocket& stream, const string& msg, PointRef pubKey, CurveRef curve)
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
    
    PointDestroy(kp);
	
	stream.send(pkt);
}

bool dsa_receive_and_verify_message(TcpSocket& stream, CurveRef curve, PointRef pubKey)
{
	bool verified = false;
	
	// message (string)
	// u (mpz_t)
	// v (mpz_t)
	
	string messageString;
	string uString;
	string vString;
	
	Packet pkt;
	stream.receive(pkt);
	
	pkt >> messageString;
	pkt >> uString;
	pkt >> vString;
	
	mpz_t u, v;
	mpz_init_set_str(u, uString.c_str(), 10);
	mpz_init_set_str(v, vString.c_str(), 10);
	
	verified = dsa_verify_message(messageString.data(), messageString.size(), u, v, curve, pubKey);
	mpz_clears(u, v);
	
	return verified;
}

void dsa_sign_and_send_message(TcpSocket& stream, const string& msg, mpz_t secret, CurveRef curve)
{
	mpz_t u, v;
	dsa_sign_message(msg.data(), msg.size(), u, v, secret, curve);
    
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

bool dsa_verify_message(const void *data, size_t dataLength, mpz_t u, mpz_t v, CurveRef curve, PointRef pubKey)
{
	bool verified = false;
	
	bool uvInBound = true;
	uvInBound = uvInBound && mpz_cmp_si(u, 0) > 0;
	uvInBound = uvInBound && mpz_cmp(u, curve->n) < 0;
	uvInBound = uvInBound && mpz_cmp_si(v, 0) > 0;
	uvInBound = uvInBound && mpz_cmp(v, curve->n) < 0;
	
	if (uvInBound)
	{
		PointRef p = curve->g;
		PointRef q = pubKey;
		PointRef p1 = NULL;
		PointRef p2 = NULL;
		PointRef xy = NULL;
		
		mpz_t hm, invV, lmon, rmon;
		mpz_inits(hm, invV, lmon, rmon, NULL);
		
		// Compute left monomial: (H(m)/v mod n).P
		sha256(hm, data, dataLength);
		mpz_invert(invV, v, curve->n);
		mpz_mul(lmon, hm, invV);
		mpz_mod(lmon, lmon, curve->n);
		p1 = PointCreateMultiple(p, lmon, curve);
		
		// Compute right monomial: (u/v mod n).Q
		mpz_mul(rmon, u, invV);
		mpz_mod(rmon, rmon, curve->n);
		p2 = PointCreateMultiple(q, rmon, curve);
		
		xy = PointCreateAdd(p1, p2, curve);
		
		PointDestroy(p1), p1 = NULL;
		PointDestroy(p2), p2 = NULL;
		mpz_clears(hm, invV, lmon, rmon, NULL);
		
		if (mpz_cmp(u, xy->x) == 0)
		{
			PointRef nullPoint = PointCreateFromInt(0, 0);
			
			if (PointEqual(nullPoint, q) == false)
			{
				if (PointIsOnCurve(q, curve))
				{
					
					PointRef m = PointCreateMultiple(q, curve->n, curve);
					
					if (PointEqual(m, nullPoint))
					{
						verified = true;
					}
					
					PointDestroy(m), m = NULL;
				}
			}
			
			PointDestroy(nullPoint), nullPoint = NULL;
		}
	}
	
	return verified;
}

void dsa_sign_message(const void *data, size_t data_length, mpz_t& u, mpz_t& v, mpz_t secret, CurveRef curve)
{
    mpz_inits(u, v, NULL);
    
    do {
        mpz_t k;
		mpz_t msgHashed;
		mpz_t kinv;
		
        secure_rand(k, curve->n);
        PointRef p = curve->g;
        PointRef kp = PointCreateMultiple(p, k, curve);
		mpz_inits(u, v, msgHashed, kinv, NULL);
		
        mpz_mod(u, kp->x, curve->n);
        
        sha256(msgHashed, data, data_length);
        
        mpz_mul(v, secret, u);
        mpz_mod(v,v,curve->n);
        
        mpz_add(v, msgHashed, v);
        mpz_mod(v,v, curve->n);
        
        mpz_invert(kinv, k, curve->n);
        
        mpz_mul(v, v, kinv);
        mpz_mod(v, v, curve->n);
        
		PointDestroy(kp);
		mpz_clears(k, msgHashed, kinv, NULL);
    } while ((mpz_cmp_ui(u, 0) == 0) && (mpz_cmp_ui(v, 0) == 0));
}
