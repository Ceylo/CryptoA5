//
//  server.cpp
//  CryptoA5
//
//  Created by Baptiste on 28/10/13.
//  Copyright (c) 2013 Yalir. All rights reserved.
//

#include "server.h"
#include "util.h"
#include "cipher.h"
#include "transmission.h"
#include <string>
#include <iostream>

using namespace std;

void server()
{
	sf::TcpListener listener;
	
	// bind the listener to a port
	if (listener.listen(DHSTS_PORT) != sf::Socket::Done)
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
    
    string curveData = send_random_curve(socket);
	
	// Load curve
	CurveRef curve = CurveCreateFromData(curveData.c_str());
	mpz_t x;
    
    PointRef q = create_key(socket, curve, x);
    send_key(socket, q);
	
	PointRef pubKey = receive_key(socket);
    
    PointRef sharedSecret = PointCreateMultiple(pubKey, x, curve);

	
    mpz_clear(x);
    socket.disconnect();
}