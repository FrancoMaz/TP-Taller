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
	string nombre;
	TexturaSDL* texturaJugador;
	VistaJugador(string nomb, int x, int y, TexturaSDL* textura);
	VistaJugador();
	virtual ~VistaJugador();
};

#endif /* VISTAJUGADOR_H_ */
