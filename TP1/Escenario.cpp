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
