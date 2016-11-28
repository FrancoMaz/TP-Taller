/*
 * Personaje.h
 *
 *  Created on: 13 nov. 2016
 *      Author: josefina
 */

#ifndef PERSONAJE_H_
#define PERSONAJE_H_
#include "Arma.h"
#include <vector>

using namespace std;

class Personaje {
protected:
	int vida;
	vector<Arma*> armas;
	bool estaMuerto;
	bool disparar;
public:
	Personaje();
	virtual ~Personaje();
	virtual void daniarseCon(int danio);
	virtual bool getEstaMuerto();
	virtual bool estaDisparando();
	virtual Proyectil* dispararProyectil();
	virtual string getNombre();
};

#endif /* PERSONAJE_H_ */
