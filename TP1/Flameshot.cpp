/*
 * Flameshot.cpp
 *
 *  Created on: 13 nov. 2016
 *      Author: josefina
 */

#include "Flameshot.h"

Flameshot::Flameshot() {
	this->disparoDiagonal = false;
	this->cortoAlcance = false;
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
					posX = posicionX+187;
					posY = posicionY+115;
				} else {
					posX = posicionX-85;
					posY = posicionY+115;
				}
			} else {
				if (condicion == "Normal"){
					posX = posicionX+187;
					posY = posicionY+140;
				} else {
					posX = posicionX-85;
					posY = posicionY+140;
				}
			}

			if(condicion == "Normal"){
				this->boxCollider = {posX,posY+18,4,50};
			} else {
				this->boxCollider = {posX+111,posY+18,4,50};
			}
			break;
		case 1:
			if (condicion == "Normal"){
				posX = posicionX+60;
				posY = posicionY-50;
			} else {
				posX = posicionX+50;
				posY = posicionY-50;
			}
			this->boxCollider = {posX,posY+111,50,4};
			break;
	}
	//this->proyectil = new Proyectil(35, 40, this->nombreArma, posX, posY, condicion, anguloBala, this->boxCollider, this->cortoAlcance, disparadoPor, nombreJugador);
	this->municiones -= 1;
	//return this->proyectil;
	// el 2 se le pasa porque es el numero en el vector de sprites
	return new Proyectil(35, 40, "2", posX, posY, condicion, anguloBala, this->boxCollider, this->cortoAlcance, disparadoPor, nombreJugador, make_pair(0,0));
}

bool Flameshot::sinMuniciones()
{
	return (this->municiones == 0);
}

void Flameshot::sumarMuniciones()
{
	this->municiones += this->municionesPorRepuesto;
}
