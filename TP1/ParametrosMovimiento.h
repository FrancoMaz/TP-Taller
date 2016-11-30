/*
 * ParametrosMovimiento.h
 *
 *  Created on: 9 nov. 2016
 *      Author: chris
 */

#ifndef PARAMETROSMOVIMIENTO_H_
#define PARAMETROSMOVIMIENTO_H_
#include "Jugador.h"
#include "Servidor.h"
#include "Enemigo.h"
#include "Boss.h"

using namespace std;

class ParametrosMovimiento {

public:
	Servidor* servidor;
	Jugador* jugador;
	Proyectil* proyectil;
	Enemigo* enemigo;
	string nombrePersonaje;
	Boss* boss;
	Personaje * personaje;
	ParametrosMovimiento(Servidor* servidorActual, Jugador* jugadorActual);
	virtual ~ParametrosMovimiento();
};

#endif /* PARAMETROSMOVIMIENTO_H_ */
