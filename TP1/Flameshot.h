/*
 * Flameshot.h
 *
 *  Created on: 13 nov. 2016
 *      Author: josefina
 */

#ifndef FLAMESHOT_H_
#define FLAMESHOT_H_

#include "Arma.h"

class Flameshot: public Arma {
public:
	Flameshot();
	virtual ~Flameshot();
	Proyectil* disparar(SDL_Rect boxCollider, string condicion, int anguloBala);
	bool sinMuniciones();
	void sumarMuniciones();
};


#endif /* FLAMESHOT_H_ */
