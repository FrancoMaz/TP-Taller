/*
 * VistaBala.cpp
 *
 *  Created on: 20 nov. 2016
 *      Author: franco
 */

#include "VistaBala.h"

VistaBala::VistaBala(int x, int y, TexturaSDL* textura, int id, string sentido) {
	this->x = x;
	this->y = y;
	this->textura = textura;
	this->id = id;
	if (sentido == "Normal")
	{
		this->flip = SDL_FLIP_NONE;
	}
	else
	{
		this->flip = SDL_FLIP_HORIZONTAL;
	}
}

VistaBala::~VistaBala() {
	// TODO Auto-generated destructor stub
}

