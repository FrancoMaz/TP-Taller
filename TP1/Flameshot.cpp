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
	this->boxCollider;
}

Flameshot::~Flameshot() {
	// TODO Auto-generated destructor stub
}

Proyectil* Flameshot::disparar(int posicionX,int posicionY, string condicion, int anguloBala, bool agachar, int disparadoPor,string nombreJugador) {
	int posX, posY;
	switch (anguloBala){
		case 0:
			if (!agachar){
				if (condicion == "Normal"){
					posX = posicionX+183;
					posY = posicionY+120;
				} else {
					posX = posicionX-332;
					posY = posicionY+120;
				}
			} else {
				if (condicion == "Normal"){
					posX = posicionX+173;
					posY = posicionY+145;
				} else {
					posX = posicionX-332;
					posY = posicionY+145;
				}
			}
			break;
		case 1:
			if (condicion == "Normal"){
				posX = posicionX-60;
				posY = posicionY-154;
			} else {
				posX = posicionX-80;
				posY = posicionY-154;
			}
			break;
	}
	this->boxCollider = {posX,posY,91,80};
	//this->proyectil = new Proyectil(35, 40, this->nombreArma, posX, posY, condicion, anguloBala, this->boxCollider, this->cortoAlcance, disparadoPor, nombreJugador);
	this->municiones -= 1;
	//return this->proyectil;
	return new Proyectil(35, 40, this->nombreArma, posX, posY, condicion, anguloBala, this->boxCollider, this->cortoAlcance, disparadoPor, nombreJugador,make_pair(0,0));
}

bool Flameshot::sinMuniciones()
{
	return (this->municiones == 0);
}

void Flameshot::sumarMuniciones()
{
	this->municiones += this->municionesPorRepuesto;
}
