/*
 * VistaJugador.cpp
 *
 *  Created on: Oct 18, 2016
 *      Author: franco
 */

#include "VistaJugador.h"

VistaJugador::VistaJugador(string nomb, int xVal, int yVal, TexturaSDL* textura, SDL_RendererFlip flipRender,TexturaSDL* _energia, TexturaSDL* imgEnergia, TexturaSDL* _puntaje, TexturaSDL* imgPuntaje, TexturaSDL* _texturaNombre) {
	valorEnergia = 100;
	valorPuntaje = 0;
	nombre = nomb;
	x = xVal;
	y = yVal;
	texturaJugador = textura;
	flip = flipRender;
	imagenPuntaje = imgPuntaje;
	puntaje = _puntaje;
	energia = _energia;
	imagenEnergia = imgEnergia;
	texturaNombre = _texturaNombre;
}

VistaJugador::VistaJugador(){

}

VistaJugador::~VistaJugador() {
	// TODO Auto-generated destructor stub
}
