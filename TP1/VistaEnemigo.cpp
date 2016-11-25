/*
 * VistaEnemigo.cpp
 *
 *  Created on: 25 nov. 2016
 *      Author: josefina
 */

#include "VistaEnemigo.h"

VistaEnemigo::VistaEnemigo(int x, int y, TexturaSDL* textura, int id) {
	this->x = x;
	this->y = y;
	this->textura = textura;
	this->id = id;
}

VistaEnemigo::~VistaEnemigo() {
	// TODO Auto-generated destructor stub
}

