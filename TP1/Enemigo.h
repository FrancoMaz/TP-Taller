/*
 * Enemigo.h
 *
 *  Created on: 13 nov. 2016
 *      Author: josefina
 */

#ifndef ENEMIGO_H_
#define ENEMIGO_H_

#include "HeavyMachineGun.h"
#include "Personaje.h"

class Enemigo: public Personaje {
private:
	int id;
public:
	Enemigo();
	virtual ~Enemigo();
	void disparar();
	//void daniarseCon(Proyectil* proyectil);
	void setId(int id);
};

#endif /* ENEMIGO_H_ */
