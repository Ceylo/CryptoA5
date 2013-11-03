//
//  transmission.h
//  CryptoA5
//
//  Created by Baptiste on 28/10/13.
//  Copyright (c) 2013 Yalir. All rights reserved.
//

#ifndef __CryptoA5__transmission__
#define __CryptoA5__transmission__

#include "ec_operations.h"
#include <SFML/Network.hpp>
#include <gmp.h>

using namespace sf;
using namespace std;

#define DH_PORT 4444
#define EG_PORT 4445
#define DHSTS_PORT 4445

/** Create the a.P key and send it to the peer
 *
 * The resulting point is returned and the a used to generate the key
 * stored in outA
 */
PointRef create_key(CurveRef curve, mpz_t& outA);

/** Send the a.P key which is the public key
 */
void send_key(TcpSocket& stream, PointRef p);

/** Receive the remote a.P key
 */
PointRef receive_key(TcpSocket& stream);

/** Send the curve data (raw representation of the curve file) from a random choosen curve
 */
string send_random_curve(TcpSocket& stream);

/** Receive the curve data (raw representation of the curve file) and
 * create a curve object from it
 *
 * @param stream the network stream to read
 * @return the received curve
 */
CurveRef receive_curve(TcpSocket& stream);

#endif /* defined(__CryptoA5__transmission__) */
