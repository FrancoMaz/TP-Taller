/*
 * TaniOh.h
 *
 *  Created on: 28 nov. 2016
 *      Author: franco
 */

#ifndef TANIOH_H_
#define TANIOH_H_
#include "Boss.h"
#define VELOCIDADTANIOH 3

class TaniOh: public Boss {
public:
	TaniOh(string nombre, string x, string y);
	virtual ~TaniOh();
	virtual void comportamiento(SDL_Rect camara, bool tieneQueDisparar);
};

#endif /* TANIOH_H_ */
