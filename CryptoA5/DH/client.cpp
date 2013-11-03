
//
//  client.cpp
//  DH
//
//  Created by Ceylo on 21/10/2013.
//  Copyright (c) 2013 Yalir. All rights reserved.
//


#include "client.h"
#include "util.h"
#include "network.h"
#include <string>
#include <iostream>

using namespace std;

CurveRef client_receive_curve(TcpSocket& stream)
{
	Packet pkt;
	stream.receive(pkt);
	
	string curveData;
	pkt >> curveData;
	
	return CurveCreateFromData(curveData.c_str());
}

PointRef client_create_key(TcpSocket& stream, CurveRef curve, mpz_t& outB)
{
	secure_rand(outB, curve->n);
	return PointCreateMultiple(curve->g, outB, curve);
}

void client_send_key(TcpSocket& stream, PointRef p)
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
//	cout << "Sent (" << bgxString << ", " << bgyString << ")" << endl;
}

PointRef client_receive_key(TcpSocket& stream)
{
	Packet pkt;
	stream.receive(pkt);
	
	string peerXString, peerYString;
	pkt >> peerXString;
	pkt >> peerYString;
	
	mpz_t peerX;
	mpz_t peerY;
	
	mpz_init_set_str(peerX, peerXString.c_str(), 10);
	mpz_init_set_str(peerY, peerYString.c_str(), 10);
	
	PointRef peerKey = PointCreateFromGMP(peerX, peerY);
	mpz_clears(peerX, peerY, NULL);
	
//	cout << "Received remote key: " << PointCreateDescription(peerKey) << endl;
	
	return peerKey;
}

void client()
{
	TcpSocket socket;
	Socket::Status status = socket.connect(IpAddress::getLocalAddress(), DH_PORT);
	
	if (status != Socket::Status::Done)
	{
		perror("connection to server failed");
		return;
	}
	
	mpz_t secret;
	CurveRef curve = client_receive_curve(socket);
	PointRef p = client_create_key(socket, curve, secret);
	client_send_key(socket, p);
	PointRef remoteKey = client_receive_key(socket);
	PointRef sharedSecret = PointCreateMultiple(remoteKey, secret, curve);
	
	char *desc = PointCreateDescription(sharedSecret);
	cout << "Secret: " << desc << endl;
    free(desc);
	
    mpz_clear(secret);
    CurveDestroy(curve);
    PointDestroy(p);
    PointDestroy(remoteKey);
    PointDestroy(sharedSecret);
	
	socket.disconnect();
}
