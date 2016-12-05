/*
 * AirbusterRiberts.h
 *
 *  Created on: 28 nov. 2016
 *      Author: franco
 */

#ifndef AIRBUSTERRIBERTS_H_
#define AIRBUSTERRIBERTS_H_
#define VELOCIDADAIRBUSTER 6
#include "Boss.h"

class AirbusterRiberts: public Boss {
public:
	AirbusterRiberts(string nombre, string x, string y);
	virtual ~AirbusterRiberts();
	void comportamiento(SDL_Rect camara,  bool tieneQueDisparar);
	Enemigo* liberarSoldado(int xJugador);
};

#endif /* AIRBUSTERRIBERTS_H_ */
