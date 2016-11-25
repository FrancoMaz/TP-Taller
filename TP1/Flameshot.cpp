/*
 * Flameshot.cpp
 *
 *  Created on: 13 nov. 2016
 *      Author: josefina
 */

#include "Flameshot.h"

Flameshot::Flameshot() {
	this->disparoDiagonal = false;
	this->cortoAlcance = true;
	this->municiones = 30;
	this->nombre = "_lanzallamas_";
	this->municionesPorRepuesto = 15;
	this->nombreArma = "FlameShot";
}

Flameshot::~Flameshot() {
	// TODO Auto-generated destructor stub
}

Proyectil* Flameshot::disparar(SDL_Rect boxCollider, string condicion) {
	this->proyectil = new Proyectil(35,40,this->nombreArma,boxCollider,condicion);
	this->municiones -= 1;
	return this->proyectil;
}

bool Flameshot::sinMuniciones()
{
	return (this->municiones == 0);
}

void Flameshot::sumarMuniciones()
{
	this->municiones += this->municionesPorRepuesto;
}
