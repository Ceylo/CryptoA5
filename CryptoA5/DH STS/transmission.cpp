//
//  transmission.cpp
//  CryptoA5
//
//  Created by Baptiste on 28/10/13.
//  Copyright (c) 2013 Yalir. All rights reserved.
//

#include "transmission.h"
#include <string>
#include "util.h"
#include <iostream>

PointRef create_key(TcpSocket& stream, CurveRef curve, mpz_t& outA)
{
	gmp_printf("%Zd\n", curve->n);
	secure_rand(outA, curve->n);
    
    return PointCreateMultiple(curve->g, outA, curve);
}

void send_key(TcpSocket& stream, PointRef p)
{
	char *bgxBuffer = NULL;
	char *bgyBuffer = NULL;
	string bgxString, bgyString;
	Packet pkt;
	
	gmp_asprintf(&bgxBuffer, "%Zd", p->x);
	gmp_asprintf(&bgyBuffer, "%Zd", p->y);
	
	bgxString = string(bgxBuffer);
	bgyString = string(bgyBuffer);
	free(bgxBuffer), bgxBuffer = NULL;
	free(bgyBuffer), bgyBuffer = NULL;
	
	pkt << bgxString;
	pkt << bgyString;
	
	stream.send(pkt);
	cout << "Sent (" << bgxString << ", " << bgyString << ")" << endl;
}

PointRef receive_key(TcpSocket& stream)
{
  	Packet pkt;
	stream.receive(pkt);
	
	string peerXString, peerYString;
	pkt >> peerXString;
	pkt >> peerYString;
	
	mpz_t peerX;
	mpz_t peerY;
	
	mpz_init_set_str(peerX, peerXString.c_str());
	mpz_init_set_str(peerY, peerYString.c_str());
	
	PointRef peerKey = PointCreateFromGMP(peerX, peerY);
	mpz_clears(peerX, peerY, NULL);
	
	cout << "Received remote key: " << PointCreateDescription(peerKey) << endl;
	
	return peerKey;
}