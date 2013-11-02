//
//  sts.h
//  CryptoA5
//
//  Created by Ceylo on 02/11/2013.
//  Copyright (c) 2013 Yalir. All rights reserved.
//

#ifndef __CryptoA5__sts__
#define __CryptoA5__sts__

#include "ec_operations.h"
#include <SFML/Network.hpp>
using namespace sf;

typedef struct {
	TcpSocket stream;
	
	mpz_t dsaRand, dhRand;
	CurveRef curve;
	PointRef myDsaPubKey, myDhPubKey;
	PointRef peerDsaPubKey, peerDhPubKey;
	PointRef k;
} STSContext;

void sts_from_curve_maker(STSContext& context);
void sts_from_curve_receiver(STSContext& context);

void maker_setup(STSContext& context);
void receiver_setup(STSContext& context);
void diffie_hellman(STSContext& context);
void show_my_idendity(STSContext& context);
void verify_peer_identity(STSContext& context);

#endif /* defined(__CryptoA5__sts__) */
