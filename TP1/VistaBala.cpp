/*
 * VistaBala.cpp
 *
 *  Created on: 20 nov. 2016
 *      Author: franco
 */

#include "VistaBala.h"

VistaBala::VistaBala(int x, int y, TexturaSDL* textura, int id) {
	this->x = x;
	this->y = y;
	this->textura = textura;
	this->id = id;
}

VistaBala::~VistaBala() {
	// TODO Auto-generated destructor stub
}

