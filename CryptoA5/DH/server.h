#include <SFML/Network.hpp>
#include "ec_operations.h"
#include <string>

using namespace sf;
using namespace std;


string server_send_random_curve(TcpSocket& stream);
PointRef server_create_key(TcpSocket& stream, CurveRef curve, mpz_t& outA);
void server_send_key(TcpSocket& stream, PointRef p);
PointRef server_receive_key(TcpSocket& stream);

void server();