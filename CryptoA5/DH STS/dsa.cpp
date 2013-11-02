//
//  dsa.cpp
//  CryptoA5
//
//  Created by Baptiste on 28/10/13.
//  Copyright (c) 2013 Yalir. All rights reserved.
//

#include "dsa.h"
#include "util.h"
#include <iostream>

bool verify_message(const void *data, size_t dataLength, mpz_t u, mpz_t v, CurveRef curve, PointRef pubKey)
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

void sign_message(const void *data, size_t data_length, mpz_t& u, mpz_t& v, mpz_t secret, CurveRef curve)
{
    mpz_inits(u, v, NULL);
    
    do {
        mpz_t k;
        secure_rand(k, curve->n);
        PointRef p = curve->g;
        PointRef kp = PointCreateMultiple(p, k, curve);
        
        mpz_init(u);
        mpz_mod(u, kp->x, curve->n);
        
        mpz_t msgHashed;
        mpz_inits(v, msgHashed, NULL);
        
        sha256(msgHashed, data, data_length);
        
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
}