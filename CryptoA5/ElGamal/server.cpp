//
//  server.cpp
//  CryptoA5
//
//  Created by Baptiste on 21/10/13.
//  Copyright (c) 2013 Yalir. All rights reserved.
//

#include "server.h"
#include "util.h"
#include "crypto.h"
#include "network.h"
#include <string>
#include <iostream>

using namespace std;

void server()
{
	sf::TcpListener listener;
	
	// bind the listener to a port
	if (listener.listen(EG_PORT) != sf::Socket::Done)
	{
		perror("error when listening");
		return;
	}
	
    while(1)
    {
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
        mpz_t a;
        
        PointRef q = create_key(curve, a);
        
        send_key(socket, q);
        
        string msg = eg_receive_and_decrypt_message(socket, a, curve);
        
        cout << "Received: " << msg << endl;
        
        mpz_clear(a);
        CurveDestroy(curve);
        PointDestroy(q);
        
        socket.disconnect();
    }
	listener.close();
}
