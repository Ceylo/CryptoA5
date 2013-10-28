//
//  client.cpp
//  CryptoA5
//
//  Created by Baptiste on 28/10/13.
//  Copyright (c) 2013 Yalir. All rights reserved.
//

#include <stdio.h>
#include "util.h"
#include "cipher.h"
#include "transmission.h"
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

void client()
{
	TcpSocket socket;
	Socket::Status status = socket.connect(IpAddress::getLocalAddress(), DHSTS_PORT);
	
	if (status != Socket::Status::Done)
	{
		perror("connection to server failed");
		return;
	}
    
    mpz_t y;
    
	CurveRef curve = client_receive_curve(socket);
    PointRef pubKey = receive_key(socket);
    
    PointRef q = create_key(curve, y);
    
    PointRef sharedSecret = PointCreateMultiple(pubKey, y, curve);
    
    mpz_clear(y);
    socket.disconnect();
}