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

Proyectil* HeavyMachineGun::disparar(int posicionX,int posicionY, string condicion, int anguloBala, bool agachar, int disparadoPor, string nombreJugador) {
	int posX, posY;
	switch (anguloBala){
		case 0:
			if (!agachar){
				if (condicion == "Normal"){
					posX = posicionX+183;
					posY = posicionY+143;
				} else {
					posX = posicionX+11;
					posY = posicionY+143;
				}
			} else {
				if (condicion == "Normal"){
					posX = posicionX+183;
					posY = posicionY+174;
				} else {
					posX = posicionX+11;
					posY = posicionY+174;
				}
			}
			break;
		case 1:
			if (condicion == "Normal"){
				posX = posicionX+90;
				posY = posicionY+43;
			} else {
				posX = posicionX+104;
				posY = posicionY+43;
			}
			break;
		case 2:
			if (condicion == "Normal"){
				posX = posicionX+175;
				posY = posicionY+118;
			} else {
				posX = posicionX+32;
				posY = posicionY+118;
			}
			break;
	}
	this->boxCollider = {posX,posY,22,13};
	//this->proyectil = new Proyectil(20, 15, this->nombreArma,posX,posY, condicion, anguloBala, this->boxCollider, this->cortoAlcance, disparadoPor, nombreJugador);
	this->municiones -= 1;
	//return this->proyectil;
	return new Proyectil(20, 15, this->nombreArma,posX,posY, condicion, anguloBala, this->boxCollider, this->cortoAlcance, disparadoPor, nombreJugador);
}

bool HeavyMachineGun::sinMuniciones()
{
	return (this->municiones == 0);
}

void HeavyMachineGun::sumarMuniciones()
{
	this->municiones += this->municionesPorRepuesto;
}
