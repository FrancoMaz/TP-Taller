/*
 * RocketLauncher.cpp
 *
 *  Created on: 13 nov. 2016
 *      Author: josefina
 */

#include "RocketLauncher.h"

RocketLauncher::RocketLauncher() {
	this->disparoDiagonal = false;
	this->municiones = 30;
}

RocketLauncher::~RocketLauncher() {
	// TODO Auto-generated destructor stub
}

Proyectil* RocketLauncher::disparar(SDL_Rect boxCollider) {
	this->proyectil = new Proyectil(80,40,"Rocket",boxCollider);
	this->municiones -= 1;
	return this->proyectil;
}

bool RocketLauncher::sinMuniciones()
{
	return (this->municiones == 0);
}

Proyectil* RocketLauncher::getProyectil()
{
	return this->proyectil;
}
