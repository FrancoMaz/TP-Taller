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
	agachar = false;
	disparar = false;
	restablecerPosicionSprite = false;
	angulo = 0;
}

Jugador::Jugador(string nombre, string equipo, int posicionX) {
	this->nombre = nombre;
	this->equipo = equipo;
	posicion.first = posicionX;
	posicion.second = 415;
	velocidades.first = 0;
	velocidades.second = 0;
	saltar = false;
	agachar = false;
	disparar = false;
	restablecerPosicionSprite = false;
	angulo = 0;
	condicionSprite = "Normal";
	spriteAEjecutar = "Jugador_" + equipo;
}
Jugador::~Jugador() {
	// TODO Auto-generated destructor stub
}

void Jugador::actualizarPosicion(SDL_Keycode tecla, bool sePresionoTecla, SDL_Rect camara)
{
	int posicionY = 415;

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
		if (!agachar){
			velocidades.first += velocidad;
			if (!saltar) {
				posicion.second = posicionY;
				spriteAEjecutar = "Jugador_corriendo_" + this->equipo;
			}
		}
		condicionSprite = "Normal";
	}
	else if (tecla == SDLK_LEFT && sePresionoTecla)
	{
		if (!agachar){
			velocidades.first -= velocidad;
			if (!saltar) {
				posicion.second = posicionY;
				spriteAEjecutar = "Jugador_corriendo_" + this->equipo;
			}
		}
		condicionSprite = "Espejado";
	}
	else if (tecla == SDLK_UP && sePresionoTecla)
	{
		if(angulo == 0)
		{
			saltar = true;
			posicion.second = posicionY;
		}
		spriteAEjecutar = "Jugador_saltando_" + this->equipo;
	}
	else if(tecla == SDLK_DOWN && sePresionoTecla)
	{
		if (!saltar && !disparar)
		{
			agachar = true;
			velocidades.first = 0;
			posicion.second = posicionY+40;
			spriteAEjecutar = "Jugador_agachado_" + this->equipo;
		}
	}
	else if(tecla == SDLK_SPACE && sePresionoTecla)
	{
		if (!saltar)
		{
			if (!disparar) {
				disparar = true;
				if(!agachar){
					if(condicionSprite == "Espejado"){
						posicion.first = posicion.first - 55;
						restablecerPosicionSprite = true;
					}
					spriteAEjecutar = "Jugador_disparando_" + this->equipo;
				} else {
					if(condicionSprite == "Espejado"){
						posicion.first = posicion.first - 41;
						restablecerPosicionSprite = true;
					}
					spriteAEjecutar = "Jugador_agachado_disparando_" + this->equipo;
				}
			} else {
				if(!agachar){
					if(condicionSprite == "Espejado" && restablecerPosicionSprite){
						posicion.first = posicion.first + 55;
						restablecerPosicionSprite = false;
					}
					spriteAEjecutar = "Jugador_" + this->equipo;
				} else {
					if(condicionSprite == "Espejado" && restablecerPosicionSprite){
						posicion.first = posicion.first + 41;
						restablecerPosicionSprite = false;
					}
					spriteAEjecutar = "Jugador_agachado_" + this->equipo;
				}
			}
		}
	}
	else if(tecla == SDLK_RIGHT && !sePresionoTecla)
	{
		if (!agachar){
			velocidades.first = 0;
			if (!saltar && !agachar) {
				posicion.second = posicionY;
				spriteAEjecutar = "Jugador_" + this->equipo;
			}
		}
		condicionSprite = "Normal";
	}
	else if(tecla == SDLK_LEFT && !sePresionoTecla)
	{
		if (!agachar){
			velocidades.first = 0;
			if (!saltar && !agachar)
			{
				posicion.second = posicionY;
				spriteAEjecutar = "Jugador_" + this->equipo;
			}
		}
		condicionSprite = "Espejado";
	}
	else if(tecla == SDLK_DOWN && !sePresionoTecla)
	{
		if (!saltar)
		{
			velocidades.first = 0;
			agachar = false;
			posicion.second = posicionY;
			spriteAEjecutar = "Jugador_" + this->equipo;
		}
	}
	else if(tecla == SDLK_SPACE && !sePresionoTecla)
	{
		if (!saltar)
		{
			disparar = false;
			if(!agachar){
				if(condicionSprite == "Espejado" && restablecerPosicionSprite){
					posicion.first = posicion.first + 55;
				}
				spriteAEjecutar = "Jugador_" + this->equipo;
			} else {
				if(condicionSprite == "Espejado" && restablecerPosicionSprite){
					posicion.first = posicion.first + 41;
				}
				spriteAEjecutar = "Jugador_agachado_" + this->equipo;
			}
		}
	}

	if(saltar && tecla == SDLK_UP && sePresionoTecla)
	{
		velocidades.second = -12*cos(angulo);
		angulo += PI/25;
		if (angulo > (PI + (PI/25))){
			angulo = 0;
			saltar = false;
			if (velocidades.first != 0) {
				spriteAEjecutar = "Jugador_corriendo_" + this->equipo;
			}
			else {
				spriteAEjecutar = "Jugador_" + this->equipo;
			}
		}
	} else{
		velocidades.second = 0;
	}

	posicion.first += velocidades.first;
	posicion.second += velocidades.second;

	if (posicion.first < camara.x or posicion.first + 84 > camara.x + ANCHO_VENTANA - MARGENIZQ)
	{
		posicion.first -= velocidades.first;
	}

}

void Jugador::setPosicion(int x, int y)
{
	posicion.first = x;
	posicion.second = y;
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
	return (nombre + "|" + to_string(posicion.first) + "|" + to_string(posicion.second) + "|" + this->spriteAEjecutar);
}

bool Jugador::chequearCambiarCamara(SDL_Rect camara, int anchoVentana, pair<int,int> posicionesExtremos, int anchoSprite)
{

	int posicionMasAtras = posicionesExtremos.first;
	int posicionMasAdelante = posicionesExtremos.second;
	if (camara.x < 0){
		camara.x = 0;
		return false;
	}
	if (camara.x >= posicionMasAtras)
	{
		camara.x = posicionMasAtras;
		return false;
	}

	if (this->velocidades.first <= 0)
	{
		return false;
	}
	/*if (posicionMasAdelante > (camara.x + anchoVentana/2) and posicionMasAtras > (camara.x + MARGENIZQ))
	{
		return true;
	}*/
 	if(this->posicion.first - camara.x > anchoVentana/2 && posicionMasAtras >= (camara.x + MARGENIZQ))
 	{
 		return true;
 	}
	return false;
}

pair<int,int> Jugador::getPosicion()
{
	return posicion;
}

int Jugador::getVelocidadX()
{
	return this->velocidades.first;
}

void Jugador::resetearPosicion(int anchoCapaPrincipal)
{
	posicion.first = posicion.first - anchoCapaPrincipal;
}

void Jugador::setPosicion(int posicion)
{
	this->posicion.first = posicion;
}

void Jugador::setSprite(string sprite, bool equipo, string condicion)
{
	spriteAEjecutar = sprite;
	if (equipo && condicion != "")
	{
		spriteAEjecutar = sprite + "_" + this->equipo;
		condicionSprite = condicion;
	}
}

string Jugador::getEquipo()
{
	return this->equipo;
}
