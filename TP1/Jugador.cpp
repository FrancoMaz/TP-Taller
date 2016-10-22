/*
 * Jugador.cpp
 *
 *  Created on: 8 oct. 2016
 *      Author: franco
 */

#include "Jugador.h"

Jugador::Jugador(pair<int,int> posicionInicial) {
	posicion.first = posicionInicial.first;
	posicion.second = posicionInicial.second;
	velocidades.first = 0;
	velocidades.second = 0;
	saltar = false;
	angulo = 0;
}

Jugador::Jugador(string nombre) {
	this->nombre = nombre;
	posicion.first = 20;
	posicion.second = 415;
	velocidades.first = 0;
	velocidades.second = 0;
	saltar = false;
	angulo = 0;
	condicionSprite = "Normal";
	spriteAEjecutar = "Jugador";
}
Jugador::~Jugador() {
	// TODO Auto-generated destructor stub
}

void Jugador::actualizarPosicion(SDL_Keycode tecla, bool sePresionoTecla, SDL_Rect camara)
{
	if (velocidades.first > VELMAX)
	{
		velocidades.first = VELMAX;
	}
	else if (velocidades.first < -VELMAX)
	{
		velocidades.first = -VELMAX;
	}

	if (tecla == SDLK_RIGHT && sePresionoTecla)
	{
		velocidades.first += velocidad;
		if (!saltar) {
			spriteAEjecutar = "Jugador_corriendo";
			condicionSprite = "Normal";
		}
	}
	else if (tecla == SDLK_LEFT && sePresionoTecla)
	{
		velocidades.first -= velocidad;
		if (!saltar) {
			spriteAEjecutar = "Jugador_corriendo";
			condicionSprite = "Espejado";
		}
	}
	else if (tecla == SDLK_UP && sePresionoTecla)
	{
		if(angulo == 0)
		{
			saltar = true;
			spriteAEjecutar = "Jugador_saltando";
		}
	}
	else if(tecla == SDLK_RIGHT && !sePresionoTecla)
	{
		velocidades.first = 0;
		if (!saltar) {
			spriteAEjecutar = "Jugador";
			condicionSprite = "Normal";
		}
	}
	else if(tecla == SDLK_LEFT && !sePresionoTecla)
	{
		velocidades.first = 0;
		if (!saltar)
		{
			spriteAEjecutar = "Jugador";
			condicionSprite = "Espejado";
		}
	}

	if(saltar)
	{
		velocidades.second = -7*cos(angulo);
		angulo += PI/50;
		if (angulo > (PI + (PI/50))){
			angulo = 0;
			saltar = false;
			if (velocidades.first != 0) {
				spriteAEjecutar = "Jugador_corriendo";
			}
			else {
				spriteAEjecutar = "Jugador";
			}
		}
	} else{
		velocidades.second = 0;
	}

	posicion.first += velocidades.first;
	posicion.second += velocidades.second;

	if (posicion.first < camara.x)
	{
		posicion.first -= velocidades.first;
	}
}

void Jugador::setConectado()
{
	this->conectado = true;
}

void Jugador::setDesconectado()
{
	this->conectado = false;
}

string Jugador::getStringJugador()
{
	return ("0|" + nombre + "|" + to_string(posicion.first) + "|" + to_string(posicion.second) + "|" + spriteAEjecutar + "|" + condicionSprite + "#");
}

string Jugador::getNombre()
{
	return this->nombre;
}

string Jugador::getSpriteAEjecutar()
{
	return this->spriteAEjecutar;
}

bool Jugador::salto()
{
	return this->saltar;
}

bool Jugador::getConectado(){
	return conectado;
}

string Jugador::serializarInicio()
{
	return (nombre + "|" + to_string(posicion.first) + "|" + to_string(posicion.second));
}

bool Jugador::chequearCambiarCamara(SDL_Rect camara)
{
	if (camara.x < 0){
		camara.x = 0;
		return false;
	}
	if(this->posicion.first - camara.x > ANCHO_VENTANA/2)
	{
		return true;
	}
	return false;
}

pair<int,int> Jugador::getPosicion()
{
	return posicion;
}
