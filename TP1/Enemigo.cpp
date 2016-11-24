/*
 * Enemigo.cpp
 *
 *  Created on: 13 nov. 2016
 *      Author: josefina
 */

#include "Enemigo.h"

Enemigo::Enemigo() {
	this->vida = 100;
	this->armas.push_back(new HeavyMachineGun());
	this->estaMuerto = false;
}

Enemigo::~Enemigo() {
	// TODO Auto-generated destructor stub
}

void Enemigo::disparar() {

}

/*void Enemigo::daniarseCon(Proyectil* proyectil) {
	int danio = proyectil->danioEnPorcentaje;
	if (this->vida > 0 && danio < this->vida) {
		this->vida -= danio;
	} else {
		this->estaMuerto = true;
	}
}*/
