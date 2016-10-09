/*
 * Evento.h
 *
 *  Created on: 8 oct. 2016
 *      Author: franco
 */

#ifndef EVENTO_H_
#define EVENTO_H_

#include <SDL2/SDL.h>
#include <string>

using namespace std;

class Evento {
private:
	SDL_Keycode teclaPresionada;
	string remitente;
	string eventoSerializado;
public:
	Evento(string remitente, string eventoSerializado);
	virtual ~Evento();
	void deserializar(string evento);
	string serializar();
	SDL_Keycode getTeclaPresionada();
	string getRemitente();
};

#endif /* EVENTO_H_ */
