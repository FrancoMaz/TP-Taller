/*
 * Escenario.cpp
 *
 *  Created on: 19 nov. 2016
 *      Author: franco
 */

#include "Escenario.h"
#include <iostream>
using namespace std;

Escenario::Escenario() {
	// TODO Auto-generated constructor stub

}

Escenario::~Escenario() {
	// TODO Auto-generated destructor stub
}

void Escenario::agregarProyectil(Proyectil* proyectil, string nombreJugador, int idProyectil)
{
	this->proyectiles.push_back(proyectil);
	//proyectil->fueDisparadoPor(nombreJugador);
	proyectil->id = idProyectil;
}

bool Escenario::verificarColision(SDL_Rect camara, Proyectil* proyectil)
{
	return(proyectil->posicion.first > camara.x + camara.w || proyectil->posicion.first < camara.x);
}
