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
	TcpSocket stream;						// Network stream
	mpz_t dsaRand, dhRand;					// Numbers for private keys
	CurveRef curve;							// EC
	PointRef myDsaPubKey, myDhPubKey;		// My public keys
	PointRef peerDsaPubKey, peerDhPubKey;	// Peer public keys
	PointRef k;								// AES secret key
} STSContext;

void sts_from_curve_maker(STSContext& context);
void sts_from_curve_receiver(STSContext& context);

void maker_setup(STSContext& context);
void receiver_setup(STSContext& context);
void diffie_hellman(STSContext& context);
void show_my_idendity(STSContext& context);
void verify_peer_identity(STSContext& context);

#endif /* defined(__CryptoA5__sts__) */
