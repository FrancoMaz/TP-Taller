/*
 * TaniOh.cpp
 *
 *  Created on: 28 nov. 2016
 *      Author: franco
 */

#include "TaniOh.h"

TaniOh::TaniOh(string nombre, string x, string y) {
	this->nombre = nombre;
	this->posX = atoi(x.c_str());
	this->posY = atoi(y.c_str());
}

TaniOh::~TaniOh() {
	// TODO Auto-generated destructor stub
}

void TaniOh::comportamiento(SDL_Rect camara)
{

}
