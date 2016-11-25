/*
 * RocketLauncher.cpp
 *
 *  Created on: 13 nov. 2016
 *      Author: josefina
 */

#include "RocketLauncher.h"

RocketLauncher::RocketLauncher() {
	this->disparoDiagonal = false;
	this->cortoAlcance = false;
	this->municiones = 30;
	this->nombre = "_bazooka_";
	this->municionesPorRepuesto = 15;
	this->nombreArma = "RocketLauncher";
}

RocketLauncher::~RocketLauncher() {
	// TODO Auto-generated destructor stub
}

Proyectil* RocketLauncher::disparar(SDL_Rect boxCollider, string condicion, int anguloBala) {
	this->proyectil = new Proyectil(80,40,this->nombreArma,boxCollider,condicion,anguloBala);
	this->municiones -= 1;
	return this->proyectil;
}

bool RocketLauncher::sinMuniciones()
{
	return (this->municiones == 0);
}

void RocketLauncher::sumarMuniciones()
{
	this->municiones += this->municionesPorRepuesto;
}
