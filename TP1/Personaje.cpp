/*
 * Personaje.cpp
 *
 *  Created on: 13 nov. 2016
 *      Author: josefina
 */

#include "Personaje.h"

Personaje::Personaje() {
	// TODO Auto-generated constructor stub

}

Personaje::~Personaje() {
	// TODO Auto-generated destructor stub
}

void Personaje::disparar() {

}

void Personaje::daniarseCon(Proyectil* proyectil) {
	int danio = proyectil->getDanio();
	if (this->vida > 0 && danio < this->vida) {
		this->vida -= danio;
	} else {
		this->estaMuerto = true;
	}
}

