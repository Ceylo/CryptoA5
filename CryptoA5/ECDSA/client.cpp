//
//  client.cpp
//  CryptoA5
//
//  Created by Baptiste on 21/10/13.
//  Copyright (c) 2013 Yalir. All rights reserved.
//

#include <stdio.h>
#include "util.h"
#include "crypto.h"
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
	Socket::Status status = socket.connect(IpAddress::getLocalAddress(), EG_PORT);
	
	if (status != Socket::Status::Done)
	{
		perror("connection to server failed");
		return;
	}
    
	CurveRef curve = client_receive_curve(socket);
    PointRef pubKey = receive_key(socket);
	bool r = dsa_receive_and_verify_message(socket, curve, pubKey);
	cout << "Result: " << r << endl;
    
    CurveDestroy(curve);
    PointDestroy(pubKey);
    
    socket.disconnect();
}
