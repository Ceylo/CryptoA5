//
//  server.cpp
//  CryptoA5
//
//  Created by Baptiste on 28/10/13.
//  Copyright (c) 2013 Yalir. All rights reserved.
//

#include "sts.h"

using namespace std;

void server()
{
	STSContext context;
	sts_from_curve_maker(context);
}
