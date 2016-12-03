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
	this->boxCollider;
}

RocketLauncher::~RocketLauncher() {
	// TODO Auto-generated destructor stub
}

Proyectil* RocketLauncher::disparar(int posicionX,int posicionY, string condicion, int anguloBala, bool agachar, int disparadoPor, string nombreJugador) {
	int posX, posY;
	switch (anguloBala){
		case 0:
			if (!agachar){
				if (condicion == "Normal"){
					posX = posicionX+110;
					posY = posicionY+131;
				} else {
					posX = posicionX-10;
					posY = posicionY+131;
				}
			} else {
				if (condicion == "Normal"){
					posX = posicionX+110;
					posY = posicionY+160;
				} else {
					posX = posicionX-10;
					posY = posicionY+160;
				}
			}
			break;
		case 1:
			if (condicion == "Normal"){
				posX = posicionX+77;
				posY = posicionY+43;
			} else {
				posX = posicionX+62;
				posY = posicionY+43;
			}
			break;
	}
	this->boxCollider = {posX,posY,82,24};
	//this->proyectil = new Proyectil(80, 40, this->nombreArma, posX, posY, condicion, anguloBala, this->boxCollider, this->cortoAlcance, disparadoPor, nombreJugador);
	this->municiones -= 1;
	//return this->proyectil;
	//se le pasa un 1 al constructor de proyectil porque este es la posicion en donde se encuentra en el setSprites
	return new Proyectil(80, 40, "1", posX, posY, condicion, anguloBala, this->boxCollider, this->cortoAlcance, disparadoPor, nombreJugador);
}

bool RocketLauncher::sinMuniciones()
{
	return (this->municiones == 0);
}

void RocketLauncher::sumarMuniciones()
{
	this->municiones += this->municionesPorRepuesto;
}
