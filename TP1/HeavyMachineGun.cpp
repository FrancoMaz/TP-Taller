/*
 * HeavyMachineGun.cpp
 *
 *  Created on: 13 nov. 2016
 *      Author: josefina
 */

#include "HeavyMachineGun.h"

HeavyMachineGun::HeavyMachineGun() {
	this->disparoDiagonal = true;
	this->municiones = 200;
}

HeavyMachineGun::~HeavyMachineGun() {
	// TODO Auto-generated destructor stub
}

Proyectil* HeavyMachineGun::disparar(SDL_Rect boxCollider) {
	this->proyectil = new Proyectil(20,15,"Bala_normal",boxCollider);
	this->municiones -= 1;
	return this->proyectil;
}

bool HeavyMachineGun::sinMuniciones()
{
	return (this->municiones == 0);
}

Proyectil* HeavyMachineGun::getProyectil()
{
	return this->proyectil;
}
