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
	this->danioEnPorcentaje = 35;
	this->puntosPorDisparo = 40;
}

Flameshot::~Flameshot() {
	// TODO Auto-generated destructor stub
}

void Flameshot::disparar() {
	this->municiones -= 1;
}

bool Flameshot::sinMuniciones()
{
	return (this->municiones == 0);
}
