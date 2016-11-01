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
#include <iostream>
#include <SDL2/SDL.h>
#include "VentanaSDL.h"
#include "TexturaSDL.h"

#define velocidad 5
#define PI 3.14159265
#define VELMAX 10
#define MARGENIZQ 15
#define MARGENDER ANCHO_VENTANA/2

using namespace std;

class Jugador {
private:
	pair<int,int> posicion;
	pair<int,int> velocidades;
	string nombre;
	string equipo;
	bool saltar;
	double angulo;
	string spriteAEjecutar;
	bool conectado = false;
	string condicionSprite;

public:
	Jugador(pair<int,int> posicionInicial);
	Jugador(string nombre, string equipo, int posicionX);
	virtual ~Jugador();
	void actualizarPosicion(SDL_Keycode tecla, bool sePresionoTecla, SDL_Rect camara);
	string getStringJugador();
	string getNombre();
	string getSpriteAEjecutar();
	bool salto();
	void setDesconectado();
	void setConectado();
	bool getConectado();
	string serializarInicio();
	bool chequearCambiarCamara(SDL_Rect camara, int anchoVentana, pair<int,int> posicionesExtremos, int anchoSprite);
	pair<int, int> getPosicion();
	int getVelocidadX();
	void resetearPosicion(int anchoCapaPrincipal);
	void setPosicion(int posicion);
	void setSprite(string sprite, bool equipo=false,string condicion = "");
	string getStringJugadorDesconectado();
	string getEquipo();
	void setPosicion(int x, int y);
};
#endif /* JUGADOR_H_ */
