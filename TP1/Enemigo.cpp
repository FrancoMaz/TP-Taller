/*
 * Enemigo.cpp
 *
 *  Created on: 13 nov. 2016
 *      Author: josefina
 */

#include "Enemigo.h"

Enemigo::Enemigo(int posX, int posY, int id, int estado) {
	this->vida = 100;
	this->armas.push_back(new HeavyMachineGun());
	this->estaMuerto = false;
	this->posX = posX;
	this->posY = posY;
	this->id = id;
	this->boxCollider = {posX+86,posY,49,106};
	this->threadAsociado = false;
	this->condicionSprite = "Espejado";
	this->anguloBala = 0;
	this->agachado = false;
	if (estado == 1) {
		this->spriteEnemigo = "Enemigo_disparando";
		this->disparar = true;
	} else {
		this->spriteEnemigo = "Enemigo_quieto";
		this->disparar = false;
	}
	this->estado = estado;
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
	// le paso un 2 al final para indicar que el proyectil fue disparado por un jugador
	return (this->armas.at(0)->disparar(this->posX + 25,this->posY - 106,this->condicionSprite,anguloBala,agachado,2,""));
}

string Enemigo::getNombre() {
	return "Enemigo";
}
