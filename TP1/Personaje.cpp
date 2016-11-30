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
	for (int i = 0; i < this->armas.size(); i++) {
 		Arma* arma = this->armas.at(i);
		delete arma;
	}
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

int Personaje::getVida(){
	return this->vida;
}

string Personaje::getNombre() {

}

void Personaje::setEstaMuerto()
{
	this->estaMuerto = true;
}

void Personaje::vidaCompleta()
{
	this->vida = 100;
}
