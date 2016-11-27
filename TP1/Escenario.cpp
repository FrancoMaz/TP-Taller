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

Escenario::Escenario() {
	srand(rdtsc());
	pair<int,int> lala(rand() % 800, 415);
	for(int i=0; i < 1; i++) {
		cout << "Posicion x del enemigo: " << lala.first << endl;
		cout << "Posicion y del enemigo: " << lala.second << endl;
		this->enemigosPorNivel.push_back(lala);
	}
	this->idEnemigo = 0;
}

Escenario::~Escenario() {
	// TODO Auto-generated destructor stub
}

uint64_t Escenario::rdtsc() {
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}

void Escenario::agregarProyectil(Proyectil* proyectil, string nombreJugador, int idProyectil) {
	proyectil->id = idProyectil;
	this->proyectiles.push_back(proyectil);
	//proyectil->fueDisparadoPor(nombreJugador);
}

bool Escenario::verificarColision(SDL_Rect camara, Proyectil* proyectil)
{
	return(proyectil->posicion.first > camara.x + camara.w || proyectil->posicion.first < camara.x || proyectil->posicion.second < camara.y || proyectil->posicion.second > camara.y + camara.h);
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
		if (this->enemigosPorNivel.at(0).first < camara->x + camara->w && this->enemigosPorNivel.at(0).first > camara->x) {
			Enemigo* enemigo = new Enemigo(this->enemigosPorNivel.at(0).first,this->enemigosPorNivel.at(0).second,this->idEnemigo);
			this->enemigosActivos.push_back(enemigo);
			this->enemigosPorNivel.erase(this->enemigosPorNivel.begin() + 0);
			this->idEnemigo++;
		}
	}
}

Enemigo* Escenario::getEnemigoActivo() {
	if (!this->enemigosActivos.empty()) {
		Enemigo* enemigoPetuto = this->enemigosActivos.at(0);
		this->enemigosActivos.erase(this->enemigosActivos.begin()+0);
		return enemigoPetuto;
	} else {
		return NULL;
	}
}

bool Escenario::enemigoVivo(int idEnemigo) {
	return true;
}
