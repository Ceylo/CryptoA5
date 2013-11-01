#include <SFML/Network.hpp>
#include "ec_operations.h"
#include <string>

using namespace sf;
using namespace std;

/** Send the curve data (raw representation of the curve file) from a random choosen curve 
 */
string server_send_random_curve(TcpSocket& stream);

/** Create the a*P key where a is a randomly choosen scalaire betwen 1 and n-1
 *
 * The resulting point is returned and a which was used to generate the key is
 * stored in outA
 * a is our private key
 */
PointRef server_create_key(TcpSocket& stream, CurveRef curve, mpz_t& outA);

/** Send the a*P key
 * a*P is our public key
 */
void server_send_key(TcpSocket& stream, PointRef p);

/** Receive the b.P key
 * This is the public key of the peer
 */
PointRef server_receive_key(TcpSocket& stream);

/** Run server
 */
void server();