#include "server.h"
#include "util.h"
#include <string>
#include <iostream>

using namespace std;


string server_send_random_curve(TcpSocket& stream) {
    string curveData = readRandomCurve();
	Packet pkt;
	pkt << curveData;
	
	// Send curve
	stream.send(pkt);
    
    return curveData;
}

PointRef server_create_key(TcpSocket& stream, CurveRef curve) {
    
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
	
	// Load curve
	CurveRef curve = CurveCreateFromData(curveData.c_str());
	mpz_t a;
	gmp_printf("%Zd\n", curve->n);
	secure_rand(a, curve->n);
    
    string peerXString, peerYString;
    socket.receive(pkt);
    
    pkt >> peerXString;
    pkt >> peerYString;
    
    mpz_t peerX, peerY;
    mpz_inits(peerX, peerY, NULL);
    
    gmp_scanf(peerXString.c_str(), "%Zd", peerX);
    gmp_scanf(peerYString.c_str(), "%Zd", peerY);
    
    mpz_clears(peerX, peerY, NULL);
    
    socket.disconnect();
}