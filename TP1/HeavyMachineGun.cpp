/*
 * HeavyMachineGun.cpp
 *
 *  Created on: 13 nov. 2016
 *      Author: josefina
 */

#include "HeavyMachineGun.h"

HeavyMachineGun::HeavyMachineGun() {
	this->disparoDiagonal = true;
	this->cortoAlcance = false;
	this->municiones = 200;
	this->nombre = "_";
	this->municionesPorRepuesto = 150;
	this->nombreArma = "HeavyMachineGun";
}

HeavyMachineGun::~HeavyMachineGun() {
	// TODO Auto-generated destructor stub
}

Proyectil* HeavyMachineGun::disparar(SDL_Rect boxCollider, string condicion) {
	this->proyectil = new Proyectil(20,15,this->nombreArma,boxCollider, condicion);
	this->municiones -= 1;
	return this->proyectil;
}

bool HeavyMachineGun::sinMuniciones()
{
	return (this->municiones == 0);
}

void HeavyMachineGun::sumarMuniciones()
{
	this->municiones += this->municionesPorRepuesto;
}
