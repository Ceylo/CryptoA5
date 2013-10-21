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
#include "client.h"
#include "server.h"

using namespace sf;
using namespace std;

#define DH_PORT 4445


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

