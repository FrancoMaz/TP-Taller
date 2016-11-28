/*
 * Boss.h
 *
 *  Created on: 28 nov. 2016
 *      Author: franco
 */

#ifndef BOSS_H_
#define BOSS_H_
#include "Personaje.h"

class Boss: public Personaje {
protected:
	string nombre;
	string sentido;
	pair<int,int> velocidades;
public:
	Boss();
	virtual ~Boss();
	virtual void comportamiento(SDL_Rect camara);
	bool visto;
	int posX;
	int posY;
	string getStringBoss();
	SDL_Rect boxCollider;
};

#endif /* BOSS_H_ */
