//
//  server.cpp
//  CryptoA5
//
//  Created by Baptiste on 21/10/13.
//  Copyright (c) 2013 Yalir. All rights reserved.
//

#include "server.h"
#include "util.h"
#include <string>
#include <iostream>

using namespace std;


string server_send_random_curve(TcpSocket& stream)
{
    string curveData = readRandomCurve();
	Packet pkt;
	pkt << curveData;
	
	// Send curve
	stream.send(pkt);
    
    return curveData;
}

PointRef server_create_key(TcpSocket& stream, CurveRef curve, mpz_t& outA)
{
	gmp_printf("%Zd\n", curve->n);
	secure_rand(outA, curve->n);
    
    return PointCreateMultiple(curve->g, outA, curve);
}

void server_send_key(TcpSocket& stream, PointRef p)
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

PointRef server_receive_key(TcpSocket& stream)
{
  	Packet pkt;
	stream.receive(pkt);
	
	string peerXString, peerYString;
	pkt >> peerXString;
	pkt >> peerYString;
	
	mpz_t peerX;
	mpz_t peerY;
	
	mpz_inits(peerX, peerY, NULL);
	gmp_sscanf(peerXString.c_str(), "%Zd", &peerX);
	gmp_sscanf(peerYString.c_str(), "%Zd", &peerY);
	
	PointRef peerKey = PointCreateFromGMP(peerX, peerY);
	mpz_clears(peerX, peerY, NULL);
	
	cout << "Received remote key: " << PointCreateDescription(peerKey) << endl;
	
	return peerKey;
}

string client_decrypt_packet(Packet& pkt, mpz_t secret, CurveRef curve);
{
    string peerC1xString, peerC1yString, peerC2String;
    pkt >> peerC1xString;
    pkt >> peerC1yString;
    pkt >> peerC2String;
    
    mpz_t peerC2;
    mpz_t peerC1x, peerC1y;
    
    mpz_inits(peerC2, peerC1x, peerC1y, NULL);
    gmp_sscanf(peerC1xString.c_str(), "%Zd", &peerC1x);
    gmp_sscanf(peerC1yString.c_str(), "%Zd", &peerC1y);
	gmp_sscanf(peerC2String.c_str(), "%Zd", &peerC2);
    
    PointRef peerC1 = PointCreateFromGMP(peerC1x, peerC1y);
    mpz_clears(peerC1x, peerC1y, NULL);
    
    PointRef intermediaire = PointCreateMultiple(peerC1, secret, curve);
    
    mpz_t result;
    mpz_init(result);
    
    mpz_sub(result, peerC2, intermediaire->x); //message
    
}

void server()
{
	sf::TcpListener listener;
	
	// bind the listener to a port
	if (listener.listen(DH_PORT) != sf::Socket::Done)
	{
		perror("error when listening");
		return;
	}
	
	// accept a new connection
	sf::TcpSocket socket;
	if (listener.accept(socket) != sf::Socket::Done)
	{
		perror("error when receiving new client");
		return;
	}
    
    string curveData = server_send_random_curve(socket);
	
	// Load curve
	CurveRef curve = CurveCreateFromData(curveData.c_str());
	mpz_t a;
    
    PointRef q = server_create_key(socket, curve, a);
    
    server_send_key(socket, q);
    
    mpz_clear(a);
    
    socket.disconnect();
}
