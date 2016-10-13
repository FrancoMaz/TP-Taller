/*
 * Jugador.h
 *
 *  Created on: 8 oct. 2016
 *      Author: franco
 */

#ifndef JUGADOR_H_
#define JUGADOR_H_

#include <utility>
#include <string>
#include <SDL2/SDL.h>

using namespace std;

class Jugador {
private:
	pair<int,int> posicion;
	string nombre;

public:
	Jugador(pair<int,int> posicionInicial);
	Jugador(string nombre);
	virtual ~Jugador();
	void actualizarPosicion(SDL_Keycode teclaPresionada);
	string getStringJugador();
	string getNombre();

};

#endif /* JUGADOR_H_ */
