/*
 * Enemigo.h
 *
 *  Created on: 13 nov. 2016
 *      Author: josefina
 */

#ifndef ENEMIGO_H_
#define ENEMIGO_H_

#include "Arma.h"
#include "Personaje.h"

class Enemigo: public Personaje {
public:
	Enemigo();
	virtual ~Enemigo();
	void disparar();
	void daniarseCon(Arma* arma);
};

#endif /* ENEMIGO_H_ */
