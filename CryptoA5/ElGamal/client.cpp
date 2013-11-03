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
#include "network.h"
#include <string>
#include <iostream>

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
	eg_encrypt_and_send_message(socket, "There once was a little guy named Nino running in a large land freely. But one day he met a wolf and got eaten. END :] êœæÂê", pubKey, curve);
    
    CurveDestroy(curve);
    PointDestroy(pubKey);
    
    socket.disconnect();
}
