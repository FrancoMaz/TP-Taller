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
	int posX;
	int posY;
	string spriteEnemigo;

public:
	Enemigo(int posX, int posY, int id);
	virtual ~Enemigo();
	void disparar();
	//void daniarseCon(Proyectil* proyectil);
	void setId(int id);
	string getInformacionDelEnemigo();
};

#endif /* ENEMIGO_H_ */
