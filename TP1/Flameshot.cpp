/*
 * Flameshot.cpp
 *
 *  Created on: 13 nov. 2016
 *      Author: josefina
 */

#include "Flameshot.h"

Flameshot::Flameshot() {
	this->disparoDiagonal = false;
	this->municiones = 30;
}

Flameshot::~Flameshot() {
	// TODO Auto-generated destructor stub
}

Proyectil* Flameshot::disparar(SDL_Rect boxCollider) {
	this->proyectil = new Proyectil(35,40,"Lanzallamas",boxCollider);
	this->municiones -= 1;
	return this->proyectil;
}

bool Flameshot::sinMuniciones()
{
	return (this->municiones == 0);
}

Proyectil* Flameshot::getProyectil()
{
	return this->proyectil;
}
