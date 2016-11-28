/*
 * Escenario.cpp
 *
 *  Created on: 19 nov. 2016
 *      Author: franco
 */

#include "Escenario.h"
#include <iostream>
#include <cstdlib>

using namespace std;

Escenario::Escenario(string rutaXml) {
	XmlParser* parserNivel = new XmlParser(rutaXml);
	for(int i=0; i < 1; i++) {
		srand(rdtsc());
		pair<int,int> lala(rand() % 1200 + 800, 415);
		//pair<int,int> lala(413, 415);
		cout << "Posicion x del enemigo: " << lala.first << endl;
		this->enemigosPorNivel.push_back(lala);
	}
	this->ordenarEnemigos();
	this->idEnemigo = 0;
	this->plataformas = parserNivel->getPlataformas();
	this->itemArmas = parserNivel->getItemArmas();
	this->levelClear = false;
}

Escenario::~Escenario() {
	// TODO Auto-generated destructor stub
}

uint64_t Escenario::rdtsc() {
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}

void Escenario::ordenarEnemigos() {
	pair<int,int> posicionTemporal;
	for (int i = 0; i < this->enemigosPorNivel.size(); i++) {
		for (int j = 0; j < this->enemigosPorNivel.size() - 1; j++) {
			pair<int,int> posicion = this->enemigosPorNivel.at(j);
			pair<int,int> posicionSiguiente = this->enemigosPorNivel.at(j+1);
			if (posicion.first > posicionSiguiente.first) {
				posicionTemporal = posicion;
				this->enemigosPorNivel.at(j) = posicionSiguiente;
				this->enemigosPorNivel.at(j+1) = posicionTemporal;
			}
		}
	}
}

void Escenario::agregarProyectil(Proyectil* proyectil, string nombreJugador, int idProyectil) {
	proyectil->id = idProyectil;
	this->proyectiles.push_back(proyectil);
	//proyectil->fueDisparadoPor(nombreJugador);
}

bool Escenario::verificarColision(SDL_Rect camara, Proyectil* proyectil, bool disparando) {
	if (this->verificarColisionConEnemigo(proyectil)) {
		cout << "En verificar colision, ocurrio la colision" << endl;
		return true;
	} else {
		if (!proyectil->cortoAlcance)
		{
			return(proyectil->getBoxCollider().x > camara.x + camara.w || (proyectil->getBoxCollider().x + proyectil->getBoxCollider().w) < camara.x || (proyectil->getBoxCollider().y + proyectil->getBoxCollider().h) < camara.y || proyectil->getBoxCollider().y > camara.y + camara.h);
		}
		else
		{
			return (!disparando);
		}
	}
}

bool Escenario::verificarColisionConItem(Jugador* jugador)
{
	for (int i = 0; i < itemArmas.size(); i++)
	{
		if (this->colisionaronObjetos(jugador->boxCollider, itemArmas.at(i)->boxCollider))
		{
			jugador->obtenerMuniciones(itemArmas.at(i)->arma);
			itemArmas.at(i)->fueObtenido = true;
			return true;
		}
	}
	return false;
}

bool Escenario::verificarColisionConEnemigo(Proyectil* proyectil) {
	for (int i = 0; i < this->enemigosActivos.size(); i++) {
		//if (proyectil->posicion.first == this->enemigosActivos.at(i)->getPosX()) {
		if (this->colisionaronObjetos(proyectil->getBoxCollider(),this->enemigosActivos.at(i)->getBoxCollider()))
		{
			this->enemigosActivos.at(i)->daniarseCon(proyectil->getDanio());
			cout << "Enemigo colisiono con bala" << endl;
			return true;
		}
	}
	return false;
}

bool Escenario::colisionaronObjetos(SDL_Rect boxCollider1, SDL_Rect boxCollider2)
{
	bool colision = true;
	if ((boxCollider1.x + boxCollider1.w) < boxCollider2.x || boxCollider1.x > (boxCollider2.x + boxCollider2.w))
	{
		colision = false;
	}
	else if ((boxCollider1.y + boxCollider1.h) < boxCollider2.y || boxCollider1.y > (boxCollider2.y + boxCollider2.h))
	{
		colision = false;
	}
	return colision;
}

void Escenario::despertarEnemigos(SDL_Rect* camara) {
	if (!this->enemigosPorNivel.empty()) {
		for (int i = 0; i < 7; i++) {
			if (!this->enemigosPorNivel.empty()) {
				if (this->enemigosPorNivel.at(0).first < camara->x + camara->w && this->enemigosPorNivel.at(0).first > camara->x) {
					Enemigo* enemigo = new Enemigo(this->enemigosPorNivel.at(0).first,this->enemigosPorNivel.at(0).second,this->idEnemigo);
					this->enemigosActivos.push_back(enemigo);
					this->enemigosPorNivel.erase(this->enemigosPorNivel.begin() + 0);
					this->idEnemigo++;
				}
			}
		}
	}
}

Enemigo* Escenario::getEnemigoActivo(int posicion) {
	if (!this->enemigosActivos.empty()) {
		Enemigo* enemigoPetuto = this->enemigosActivos.at(posicion);
		//this->enemigosActivos.erase(this->enemigosActivos.begin()+0);
		//cout << "Enemigo petuto pos x: " << enemigoPetuto->getPosX() << endl;
		return enemigoPetuto;
	} else {
		return NULL;
	}
}

vector<Enemigo*> Escenario::getEnemigosActivos() {
	return this->enemigosActivos;
}

bool Escenario::enemigoPerdido(int id, SDL_Rect* camara) {
	for (int i = 0; i < this->enemigosActivos.size(); i++) {
		Enemigo* enemigo = this->enemigosActivos.at(i);
		if (enemigo->getId() == id) {
			if (this->enemigosActivos.at(i)->getPosX() >= camara->x + camara->w && this->enemigosActivos.at(i)->getPosX() <= camara->x) {
				return true;
			}
		}
	}
	return false;
}

void Escenario::eliminarEnemigoActivo(int id) {
	for (int i = 0; i < this->enemigosActivos.size(); i++) {
		Enemigo* enemigo = this->enemigosActivos.at(i);
		if (enemigo->getId() == id) {
			this->enemigosActivos.erase(this->enemigosActivos.begin() + i);
		}
	}
}
