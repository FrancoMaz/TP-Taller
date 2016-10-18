/*
 * VistaJugador.cpp
 *
 *  Created on: Oct 18, 2016
 *      Author: franco
 */

#include "VistaJugador.h"

VistaJugador::VistaJugador(string nomb, int xVal, int yVal, TexturaSDL* textura, SDL_RendererFlip flipRender) {
	nombre = nomb;
	x = xVal;
	y = yVal;
	texturaJugador = textura;
	flip = flipRender;
}

VistaJugador::VistaJugador(){

}

VistaJugador::~VistaJugador() {
	// TODO Auto-generated destructor stub
}
