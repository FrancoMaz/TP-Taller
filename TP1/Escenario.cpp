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
	this->idEnemigo = 0;
	this->plataformas = parserNivel->getPlataformas();
	this->itemArmas = parserNivel->getItemArmas();
	this->boss = parserNivel->getBoss();
	this->capas = parserNivel->getCapas();
	this->enemigosPorNivel = parserNivel->getEnemigos();
	this->levelClear = false;
	/*int posX = 1000;
	for(int i = 0; i < 15; i++) {
		pair<int,int> lala(posX, 415);
		this->enemigosPorNivel.push_back(lala);
		posX += 350;
	}
	// estos enemigos son para las plataformas del nivel 1
	pair<int,int> plataforma12(575, 354-106);
	this->enemigosPorNivel.push_back(plataforma12);
	pair<int,int> plataforma2(1630, 354-106);
	this->enemigosPorNivel.push_back(plataforma2);
	pair<int,int> plataforma3(2100, 354-106);
	this->enemigosPorNivel.push_back(plataforma3);
	pair<int,int> plataforma4(2370, 354-106);
	this->enemigosPorNivel.push_back(plataforma4);
	pair<int,int> plataforma5(2635, 354-106);
	this->enemigosPorNivel.push_back(plataforma5);
	pair<int,int> plataforma61(3500, 354-106);
	this->enemigosPorNivel.push_back(plataforma61);
	pair<int,int> plataforma62(3700, 354-106);
	this->enemigosPorNivel.push_back(plataforma62);
	pair<int,int> plataforma71(4700, 354-106);
	this->enemigosPorNivel.push_back(plataforma71);
	pair<int,int> plataforma72(5000, 354-106);
	this->enemigosPorNivel.push_back(plataforma72);
	pair<int,int> plataforma8(5900, 354-106);
	this->enemigosPorNivel.push_back(plataforma8);
	pair<int,int> plataforma9(6950, 354-106);
	this->enemigosPorNivel.push_back(plataforma9);*/
	this->ordenarEnemigos();
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

void Escenario::agregarProyectil(Proyectil* proyectil, string nombreJugador) {
	this->proyectiles.push_back(proyectil);
	//proyectil->fueDisparadoPor(nombreJugador);
}

bool Escenario::verificarColision(SDL_Rect camara, Proyectil* proyectil, bool disparando) {
	if (this->verificarColisionConEnemigo(proyectil)) {
		proyectil->colisionPersonaje = true;
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
	//pthread_mutex_lock(&mutexEnemigosActivos);
	for (int i = 0; i < this->enemigosActivos.size(); i++) {
		if(!this->enemigosActivos.at(i)->getEstaMuerto()){
			if (this->colisionaronObjetos(proyectil->getBoxCollider(),this->enemigosActivos.at(i)->getBoxCollider())) {
				this->enemigosActivos.at(i)->daniarseCon(proyectil->getDanio());
				cout << "Enemigo colisiono con bala" << endl;
				return true;
			}
		}
	}
	if (this->colisionaronObjetos(proyectil->getBoxCollider(),this->boss->boxCollider))
	{
		this->boss->daniarseCon(proyectil->getDanio());
		return true;
	}
	//pthread_mutex_unlock(&mutexEnemigosActivos);
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
				srand(rdtsc());
				int estado = rand() % 2;
				if (this->enemigosPorNivel.at(0).first < camara->x + camara->w && this->enemigosPorNivel.at(0).first > camara->x) {
					Enemigo* enemigo = new Enemigo(this->enemigosPorNivel.at(0).first,this->enemigosPorNivel.at(0).second,this->idEnemigo, estado);
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

bool Escenario::despertarBoss(SDL_Rect camara)
{
	if (this->boss != NULL)
	{
		if (this->boss->posX < camara.x + camara.w && this->boss->posX > camara.x)
		{
			boss->visto = true;
		}
	}
}
