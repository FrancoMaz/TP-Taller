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
	string condicionSprite;
	int anguloBala;
	bool agachado;

public:
	Enemigo(int posX, int posY, int id);
	virtual ~Enemigo();
	int getId();
	void setId(int id);
	string getInformacionDelEnemigo();
	SDL_Rect getBoxCollider();
	int getPosX();
	bool threadAsociado;
	Proyectil* dispararProyectil();
};

#endif /* ENEMIGO_H_ */
