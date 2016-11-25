/*
 * RocketLauncher.h
 *
 *  Created on: 13 nov. 2016
 *      Author: josefina
 */

#ifndef ROCKETLAUNCHER_H_
#define ROCKETLAUNCHER_H_

#include "Arma.h"

class RocketLauncher: public Arma {
public:
	RocketLauncher();
	virtual ~RocketLauncher();
	Proyectil* disparar(SDL_Rect boxCollider, string condicion);
	bool sinMuniciones();
	void sumarMuniciones();
};

#endif /* ROCKETLAUNCHER_H_ */
