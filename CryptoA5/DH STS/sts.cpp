//
//  sts.cpp
//  CryptoA5
//
//  Created by Ceylo on 02/11/2013.
//  Copyright (c) 2013 Yalir. All rights reserved.
//

#include "sts.h"

void sts_from_curve_maker(STSContext& context)
{
	maker_setup(context);
	diffie_hellman(context);
	show_my_idendity(context);
	verify_peer_identity(context);
}

void sts_from_curve_receiver(STSContext& context)
{
	receiver_setup(context);
	diffie_hellman(context);
	verify_peer_identity(context);
	show_my_idendity(context);
}

void maker_setup(STSContext& context)
{
	
}

void receiver_setup(STSContext& context)
{
	
}

void diffie_hellman(STSContext& context)
{
	
}

void show_my_idendity(STSContext& context)
{
	
}

void verify_peer_identity(STSContext& context)
{
	
}

