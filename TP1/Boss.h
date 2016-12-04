/*
 * Boss.h
 *
 *  Created on: 28 nov. 2016
 *      Author: franco
 */

#ifndef BOSS_H_
#define BOSS_H_
#include "Personaje.h"
#include "Enemigo.h"
#include <time.h>

class Boss: public Personaje {
protected:
	string sentido;
	pair<int,int> velocidades;
public:
	Boss();
	virtual ~Boss();
	virtual void comportamiento(SDL_Rect camara, bool tieneQueDisparar);
	bool visto;
	int posX;
	int posY;
	string getStringBoss();
	SDL_Rect boxCollider;
	bool disparando;
	pair<vector<Proyectil*>,int> proyectilesADisparar;
	string getNombre();
	int armaADisparar;
	int despX;
	int despY;
	bool disparoDoble;
	string nombre;
	int tiempoEntreArmas;
	bool liberaSoldados;
	virtual Enemigo* liberarSoldado();
};

#endif /* BOSS_H_ */
