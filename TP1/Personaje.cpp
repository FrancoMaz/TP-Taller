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

void Personaje::daniarseCon(int danio) {
	if (this->vida > 0 && danio < this->vida) {
		this->vida -= danio;
		cout << "Vida del enemigo petuto: " << vida << endl;
	} else {
		this->estaMuerto = true;
	}
}

bool Personaje::getEstaMuerto() {
	return this->estaMuerto;
}

bool Personaje::estaDisparando(){
	return this->disparar;
}

Proyectil* Personaje::dispararProyectil() {

}

string Personaje::getNombre() {

}

