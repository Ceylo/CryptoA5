
//
//  client.h
//  DH
//
//  Created by Ceylo on 21/10/2013.
//  Copyright (c) 2013 Yalir. All rights reserved.
//

#include <SFML/Network.hpp>
#include "ec_operations.h"

using namespace sf;
using namespace std;

/** Receive the curve data (raw representation of the curve file)
 */
CurveRef client_receive_curve(TcpSocket& stream);

/** Create the b.P key
 *
 * The resulting point is returned and the b used to generate the key
 * stored in outB
 */
PointRef client_create_key(TcpSocket& stream, CurveRef curve, mpz_t& outB);

/** Send the b.P key
 */
void client_send_key(TcpSocket& stream, PointRef p);

/** Receive the remote a.P key
 */
PointRef client_receive_key(TcpSocket& stream);

/** Run client
 */
void client();

/** Create a packet with the given message encrypted
 */
Packet client_encrypt_message(const string& message, PointRef sharedSecretKey, CurveRef curve);

/** Return the message decrypted from the given packet
 */
string client_decrypt_packet(Packet& pkt);
