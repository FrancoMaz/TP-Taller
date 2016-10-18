/*
 * VistaJugador.cpp
 *
 *  Created on: Oct 18, 2016
 *      Author: franco
 */

#include "VistaJugador.h"

VistaJugador::VistaJugador(string nomb, int xVal, int yVal, TexturaSDL* textura) {
	nombre = nomb;
	x = xVal;
	y = yVal;
	texturaJugador = textura;

}

VistaJugador::VistaJugador(){

}

VistaJugador::~VistaJugador() {
	// TODO Auto-generated destructor stub
}

