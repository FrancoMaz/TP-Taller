/*
 * AirbusterRiberts.cpp
 *
 *  Created on: 28 nov. 2016
 *      Author: franco
 */

#include "AirbusterRiberts.h"

AirbusterRiberts::AirbusterRiberts(string nombre, string x, string y) {
	this->nombre = nombre;
	this->posX = atoi(x.c_str());
	this->posY = atoi(y.c_str());

}

AirbusterRiberts::~AirbusterRiberts() {
	// TODO Auto-generated destructor stub
}

void AirbusterRiberts::comportamiento(SDL_Rect camara)
{

}
