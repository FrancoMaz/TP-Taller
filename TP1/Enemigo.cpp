/*
 * Enemigo.cpp
 *
 *  Created on: 13 nov. 2016
 *      Author: josefina
 */

#include "Enemigo.h"

Enemigo::Enemigo(int posX, int posY, int id) {
	this->vida = 100;
	this->armas.push_back(new HeavyMachineGun());
	this->estaMuerto = false;
	this->posX = posX;
	this->posY = posY;
	this->spriteEnemigo = "Enemigo_quieto";
	this->id = id;
	this->boxCollider = {posX+86,posY,49,106};
	this->threadAsociado = false;
	this->disparar = false;
	this->condicionSprite = "Espejado";
	this->anguloBala = 0;
	this->agachado = false;
}

Enemigo::~Enemigo() {
	/*for (int i = 0; i < this->armas.size(); i++) {
		this->armas.at(i)->~Arma();
	}*/
}

string Enemigo::getInformacionDelEnemigo() {
	return (to_string(this->posX) + "|" + to_string(this->posY) + "|" + this->spriteEnemigo + "|" + to_string(this->id) + "#");
}

int Enemigo::getId() {
	return this->id;
}

SDL_Rect Enemigo::getBoxCollider() {
	return this->boxCollider;
}

int Enemigo::getPosX() {
	return this->posX;
}

Proyectil* Enemigo::dispararProyectil() {
	return (this->armas.at(0)->disparar(this->posX,this->posY,this->condicionSprite,anguloBala,agachado));
}
