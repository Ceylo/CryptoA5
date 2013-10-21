

#include <SFML/Network.hpp>
#include "ec_operations.h"

using namespace sf;


CurveRef client_receive_curve(TcpSocket& stream);
PointRef client_create_key(TcpSocket& stream, CurveRef curve);
void client_send_key(TcpSocket& stream, PointRef p);
PointRef client_receive_key(TcpSocket& stream);

void client();

