/*
 * ParametrosMovimiento.cpp
 *
 *  Created on: 9 nov. 2016
 *      Author: chris
 */

#include "ParametrosMovimiento.h"

ParametrosMovimiento::ParametrosMovimiento(Servidor* servidorActual, Jugador* jugadorActual) {
	this->servidor = servidorActual;
	this->jugador = jugadorActual;
}

ParametrosMovimiento::ParametrosMovimiento(Servidor* servidorActual) {
	this->servidor = servidorActual;
}

ParametrosMovimiento::~ParametrosMovimiento() {
	// TODO Auto-generated destructor stub
}

