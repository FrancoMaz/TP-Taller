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
	pair<int,int> lala(rand() % 800, 520);
	for(int i=0; i < 1; i++) {
		cout << "Posicion x del enemigo: " << lala.first << endl;
		cout << "Posicion y del enemigo: " << lala.second << endl;
		this->enemigosPorNivel.push_back(lala);
	}
}

Escenario::~Escenario() {
	// TODO Auto-generated destructor stub
}

void Escenario::agregarProyectil(Proyectil* proyectil, string nombreJugador, int idProyectil) {
	proyectil->id = idProyectil;
	this->proyectiles.push_back(proyectil);
	//proyectil->fueDisparadoPor(nombreJugador);
}

bool Escenario::verificarColision(SDL_Rect camara, Proyectil* proyectil)
{
	return(proyectil->posicion.first > camara.x + camara.w || proyectil->posicion.first < camara.x);
}

void Escenario::despertarEnemigos(SDL_Rect* camara) {
	if (this->enemigosPorNivel.at(0).first < camara->x + camara->w && this->enemigosPorNivel.at(0).first > camara->x) {
		Enemigo* enemigo = new Enemigo(this->enemigosPorNivel.at(0).first,this->enemigosPorNivel.at(0).second,0);
		this->enemigosActivos.push_back(enemigo);
	}
}

Enemigo* Escenario::getEnemigoActivo() {
	if (!this->enemigosActivos.empty()) {
		return this->enemigosActivos.at(0);
	} else {
		return NULL;
	}
}
