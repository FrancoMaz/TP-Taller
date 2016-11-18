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
	this->danioEnPorcentaje = 80;
	this->puntosPorDisparo = 40;
}

RocketLauncher::~RocketLauncher() {
	// TODO Auto-generated destructor stub
}

void RocketLauncher::disparar() {
	this->municiones -= 1;
}

bool RocketLauncher::sinMuniciones()
{
	return (this->municiones == 0);
}
