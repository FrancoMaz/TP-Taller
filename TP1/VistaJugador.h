/*
 * VistaJugador.h
 *
 *  Created on: Oct 18, 2016
 *      Author: franco
 */

#ifndef VISTAJUGADOR_H_
#define VISTAJUGADOR_H_
#include <utility>
#include <string>
#include <iostream>
#include <SDL2/SDL.h>
#include "VentanaSDL.h"
#include "TexturaSDL.h"

class VistaJugador {
public:
	int x;
	int y;
	int valorPuntaje;
	int valorEnergia;
	string nombre;
	TexturaSDL* texturaNombre;
	TexturaSDL* texturaJugador;
	TexturaSDL* energia;
	TexturaSDL* imagenEnergia;
	TexturaSDL* puntaje;
	TexturaSDL* imagenPuntaje;
	string sprite;
	SDL_RendererFlip flip;
	VistaJugador(string nomb, int x, int y, TexturaSDL* textura, SDL_RendererFlip flip, TexturaSDL* energia, TexturaSDL* imagenEnergia, TexturaSDL* puntaje, TexturaSDL* imagenPuntaje, TexturaSDL* _texturaNombre);
	VistaJugador();
	virtual ~VistaJugador();
};

#endif /* VISTAJUGADOR_H_ */
