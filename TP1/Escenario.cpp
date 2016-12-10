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
	this->items = parserNivel->getItemArmas();
	this->boss.push_back(parserNivel->getBoss());
	this->capas = parserNivel->getCapas();
	this->enemigosPorNivel = parserNivel->getEnemigos();
	this->levelClear = false;
	this->ordenarEnemigos();
	this->avanzoDeNivel = false;
}

Escenario::~Escenario() {
	// TODO Auto-generated destructor stub
}

void Escenario::inicializarDatosNivel()
{

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
{	pthread_mutex_lock(&mutexItems);
	for (int i = 0; i < items.size(); i++)
	{
		if (this->colisionaronObjetos(jugador->boxCollider, items.at(i)->boxCollider))
		{
			switch (items.at(i)->idItem)
			{
				case 0:
				{
					jugador->obtenerMuniciones(items.at(i)->nombre);
					items.at(i)->fueObtenido = true;
					break;
				}
				case 1:
				{	pthread_mutex_lock(&mutexEnemigosActivos);
					for (int j = 0; j < this->enemigosActivos.size(); j++)
					{
						usleep(200);
						this->enemigosActivos.at(j)->setEstaMuerto();
					}
					pthread_mutex_unlock(&mutexEnemigosActivos);
					break;
				}
				case 2:
				{
					jugador->vidaCompleta();
					break;
				}
			}
			this->bonusColisionado = items.at(i)->idItem;
			items.at(i)->fueObtenido = true;
			pthread_mutex_unlock(&mutexItems);
			return true;

		}
	}
	pthread_mutex_unlock(&mutexItems);
	return false;

}

void Escenario::agregarItemBonus(Item* item)
{	pthread_mutex_lock(&mutexItems);
	this->items.push_back(item);
	pthread_mutex_unlock(&mutexItems);
}

bool Escenario::verificarColisionConEnemigo(Proyectil* proyectil) {
	pthread_mutex_lock(&mutexEnemigosActivos);
	for (int i = 0; i < this->enemigosActivos.size(); i++) {
		if(!this->enemigosActivos.at(i)->getEstaMuerto()){
			if (this->colisionaronObjetos(proyectil->getBoxCollider(),this->enemigosActivos.at(i)->getBoxCollider())) {
				this->enemigosActivos.at(i)->daniarseCon(proyectil->getDanio());
				cout << "Enemigo colisiono con bala" << endl;
				pthread_mutex_unlock(&mutexEnemigosActivos);
				return true;
			}
		}
	}
	pthread_mutex_unlock(&mutexEnemigosActivos);
	//no se si falta poner mutex para el vector de boss
	if (!this->boss.empty())
	{
		if (this->colisionaronObjetos(proyectil->getBoxCollider(),this->boss.at(0)->boxCollider))
		{
			this->boss.at(0)->daniarseCon(proyectil->getDanio());
			return true;
		}
	}
	//
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
	pthread_mutex_lock(&mutexEnemigosPorNivel);
	if (!this->enemigosPorNivel.empty()) {
		int cantidadEnemigos = enemigosPorNivel.size();
		for (int i = 0; i < cantidadEnemigos ; i++) {
			if (!this->enemigosPorNivel.empty()) {
				srand(rdtsc());
				int estado = rand() % 2;
				if (this->enemigosPorNivel.at(0).first < camara->x + camara->w && this->enemigosPorNivel.at(0).first > camara->x) {
					Enemigo* enemigo = new Enemigo(this->enemigosPorNivel.at(0).first,this->enemigosPorNivel.at(0).second,this->idEnemigo, estado, 100, false);
					pthread_mutex_lock(&mutexEnemigosActivos);
					this->enemigosActivos.push_back(enemigo);
					pthread_mutex_unlock(&mutexEnemigosActivos);
					this->enemigosPorNivel.erase(this->enemigosPorNivel.begin() + 0);
					this->idEnemigo++;
				}
			}
		}
	}
	pthread_mutex_unlock(&mutexEnemigosPorNivel);

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
			if (this->enemigosActivos.at(i)->getPosX() >= camara->x + camara->w || this->enemigosActivos.at(i)->getPosX() <= camara->x) {
				return true;
			}
		}
	}
	return false;
}

void Escenario::eliminarEnemigoActivo(int id) {
	pthread_mutex_lock(&mutexEnemigosActivos);
	for (int i = 0; i < this->enemigosActivos.size(); i++) {
		Enemigo* enemigo = this->enemigosActivos.at(i);
		if (enemigo->getId() == id) {
			this->enemigosActivos.erase(this->enemigosActivos.begin() + i);
			delete enemigo;
		}
	}
	pthread_mutex_unlock(&mutexEnemigosActivos);
}

bool Escenario::despertarBoss(SDL_Rect camara)
{
	if (!this->boss.empty())
	{
		if (this->boss.at(0)->posX < camara.x + camara.w && this->boss.at(0)->posX > camara.x)
		{
			boss.at(0)->visto = true;
		}
	}
}

void Escenario::vaciarVectores()
{
	this->proyectiles.clear();
	this->items.clear();
	this->plataformas.clear();
	this->capas.clear();
	this->enemigosPorNivel.clear();
	this->enemigosActivos.clear();
	this->boss.clear();
}

void Escenario::agregarEnemigoActivo(Enemigo* enemigo)
{
	pthread_mutex_lock(&mutexEnemigosActivos);
	this->enemigosActivos.push_back(enemigo);
	pthread_mutex_unlock(&mutexEnemigosActivos);
}
