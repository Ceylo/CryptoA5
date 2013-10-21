//
//  main.cpp
//  DH
//
//  Created by Ceylo on 17/10/2013.
//  Copyright (c) 2013 Yalir. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <SFML/Network.hpp>

#include "ec_operations.h"
#include "path.h"

using namespace sf;
using namespace std;

#define DH_PORT 4445

void secure_rand(mpz_t random, mpz_t max)
{
	// Is that a valid PRNG
	mpz_init(random);
	gmp_randstate_t rstate;
	gmp_randinit_default(rstate);
	gmp_randseed_ui(rstate, rand());
	mpz_urandomm(random, rstate, max);
}

long limited_rand(long min, long max)
{
	return min + rand() % (max - min + 1);
}

string readFile(string filename)
{
	char *absoluteFilename = NULL;
	PathForFile(filename.c_str(), &absoluteFilename);
	
	ifstream::pos_type size;
	char * memblock;
	string result;
	
	ifstream file (string(absoluteFilename), ios::in|ios::binary|ios::ate);
	if (file.is_open())
	{
		size = file.tellg();
		memblock = new char [size];
		file.seekg (0, ios::beg);
		file.read (memblock, size);
		file.close();
		result = memblock;
		delete[] memblock;
	}
	
	return result;
}

string readRandomCurve()
{
	long curveId = limited_rand(1, 18);
	char filename[12];
	sprintf(filename, "w256-%03ld.gp", curveId);
	
	return readFile(filename);
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
	
	Packet pkt;
	socket.receive(pkt);
	
	string curveData;
	pkt >> curveData;
	
	CurveRef curve = CurveCreateFromData(curveData.c_str());
	mpz_t b;
	secure_rand(b, curve->n);
	
	socket.disconnect();
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
	
	string curveData = readRandomCurve();
	Packet pkt;
	pkt << curveData;
	
	// Send curve
	socket.send(pkt);
	
	// Load curve
	CurveRef curve = CurveCreateFromData(curveData.c_str());
	mpz_t a;
	gmp_printf("%Zd\n", curve->n);
	secure_rand(a, curve->n);
}

void usage(const char *argv0)
{
	cout << "Usage: " << argv0 << " client | server" << endl;
}

void init(int argc, const char * argv[])
{
	srand((unsigned int)time(NULL));
	PathInit(argv[0]);
}

int main(int argc, const char * argv[])
{
	if (argc < 2)
	{
		usage(argv[0]);
		return 1;
	}
	
	init(argc, argv);
	string choice = argv[1];
	
	if (choice == "server")
	{
		server();
	}
	else if (choice == "client")
	{
		client();
	}
	else
	{
		usage(argv[0]);
		return 1;
	}
	
    return 0;
}

