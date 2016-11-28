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
	SDL_Rect boxCollider;

public:
	Enemigo(int posX, int posY, int id);
	virtual ~Enemigo();
	void disparar();
	int getId();
	void setId(int id);
	string getInformacionDelEnemigo();
	SDL_Rect getBoxCollider();
	int getPosX();
	bool threadAsociado;
};

#endif /* ENEMIGO_H_ */
