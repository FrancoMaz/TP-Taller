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

Jugador::Jugador(string nombre, string equipo, int posicionX) {
	this->nombre = nombre;
	this->equipo = equipo;
	posicion.first = posicionX;
	posicion.second = 415;
	velocidades.first = 0;
	velocidades.second = 0;
	saltar = false;
	angulo = 0;
	condicionSprite = "Normal";
	spriteAEjecutar = "Jugador_" + equipo;
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
			spriteAEjecutar = "Jugador_corriendo_" + this->equipo;
		}
		condicionSprite = "Normal";
	}
	else if (tecla == SDLK_LEFT && sePresionoTecla)
	{
		velocidades.first -= velocidad;
		if (!saltar) {
			spriteAEjecutar = "Jugador_corriendo_" + this->equipo;
		}
		condicionSprite = "Espejado";
	}
	else if (tecla == SDLK_UP && sePresionoTecla)
	{
		if(angulo == 0)
		{
			saltar = true;
		}
		spriteAEjecutar = "Jugador_saltando_" + this->equipo;
	}
	else if(tecla == SDLK_RIGHT && !sePresionoTecla)
	{
		velocidades.first = 0;
		if (!saltar) {
			spriteAEjecutar = "Jugador_" + this->equipo;
		}
		condicionSprite = "Normal";
	}
	else if(tecla == SDLK_LEFT && !sePresionoTecla)
	{
		velocidades.first = 0;
		if (!saltar)
		{
			spriteAEjecutar = "Jugador_" + this->equipo;
		}
		condicionSprite = "Espejado";
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

void Jugador::mover(SDL_Rect camara){
	if (velocidades.first > VELMAX)
	{
		velocidades.first = VELMAX;
	}
	else if (velocidades.first < -VELMAX)
	{
		velocidades.first = -VELMAX;
	}

	if (movDerecha)
	{
		velocidades.first += velocidad;
		if (!saltar) {
			spriteAEjecutar = "Jugador_corriendo_" + this->equipo;
		}
		condicionSprite = "Normal";
	}
	else if (movIzquierda)
	{
		velocidades.first -= velocidad;
		if (!saltar) {
			spriteAEjecutar = "Jugador_corriendo_" + this->equipo;
		}
		condicionSprite = "Espejado";
	}
	else if (saltar)
	{
		if(angulo == 0)
		{
			saltar = true;
		}
		spriteAEjecutar = "Jugador_saltando_" + this->equipo;
	}
	else if(!movDerecha && ultimaTeclaPresionada == SDLK_RIGHT)
	{
		velocidades.first = 0;
		if (!saltar) {
			spriteAEjecutar = "Jugador_" + this->equipo;
		}
		condicionSprite = "Normal";
	}
	else if(!movIzquierda && ultimaTeclaPresionada == SDLK_LEFT)
	{
		velocidades.first = 0;
		if (!saltar)
		{
			spriteAEjecutar = "Jugador_" + this->equipo;
		}
		condicionSprite = "Espejado";
	}

	if(saltar)
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

vector<bool> Jugador::getMov(){
	vector<bool> movimiento;
	movimiento.at(DER) = movDerecha;
	movimiento.at(IZQ) = movIzquierda;
	movimiento.at(ARRIBA) = saltar;
	return movimiento;
}

void Jugador::setMov(SDL_Keycode tecla, bool sePresiono){
	SDL_Keycode ultimaTeclaPresionada = tecla;
	if (tecla == SDLK_RIGHT){
		movDerecha = sePresiono;
	}
	else if (tecla == SDLK_LEFT){
		movIzquierda = sePresiono;
	}
	else if (tecla == SDLK_UP)
	{
		saltar = sePresiono;
	}
}

string Jugador::getEquipo()
{
	return this->equipo;
}

pthread_t Jugador::getThreadMovimiento()
{
	return thrMov;
}

void Jugador::setThreadMovimiento(pthread_t thrMovimiento){
	thrMov = thrMovimiento;
}

