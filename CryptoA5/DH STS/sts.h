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
	CurveRef curve;							// EC
	PointRef myDsaPubKey, myDhPubKey;		// My public keys
	mpz_t dsaRand, dhRand;					// Numbers for private keys
	PointRef peerDsaPubKey, peerDhPubKey;	// Peer public keys
	PointRef sharedSecret;					// AES secret key
} STSContext;


/** Perform STS on server side
 */
void sts_from_curve_maker(STSContext& context);

/** Perform STS on client side
 */
void sts_from_curve_receiver(STSContext& context);

/** Perform setup on server side:
 * - wait for a connection
 * - create an EC
 * - send the EC data
 *
 * Uses:
 * Creates: stream, curve
 */
void sts_priv_maker_setup(STSContext& context);

/** Perform setup on client side:
 * - connect to the server
 * - receive the EC data
 * - create an EC from the received data
 *
 * Uses:
 * Creates: stream, curve
 */
void sts_priv_receiver_setup(STSContext& context);

/** Perform Diffie Hellman shared secret exchange protocol
 * - create DSA and DH key pairs
 * - send public keys
 * - receive peer's public keys
 * - generate the shared secret
 *
 * Uses: stream, curve
 * Creates: myDsaPubKey, myDhPubKey, dsaRand, dhRand, peerDsaPubKey,
 * peerDhPubKey, sharedSecret
 */
void sts_priv_diffie_hellman(STSContext& context);

/** Create a signature according to STS protocol and send it
 *
 * Uses: sharedSecret, myDhPubKey, peerDhPubKey, dsaRand, curve, stream
 * Creates:
 */
void sts_priv_show_my_idendity(STSContext& context);

/** Receive a signature and verify it according to STS protocol
 *
 * Uses: sharedSecret, stream, myDhPubKey, peerDhPubKey, curve, peerDsaPubKey
 * Creates:
 */
void sts_priv_verify_peer_identity(STSContext& context);

/** Free the STSContext
 */
void sts_free_context(STSContext& context);

#endif /* defined(__CryptoA5__sts__) */
