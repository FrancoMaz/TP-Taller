/*
 * UpdateJugador.cpp
 *
 *  Created on: 22 oct. 2016
 *      Author: franco
 */

#include "UpdateJugador.h"

UpdateJugador::UpdateJugador() {
	// TODO Auto-generated constructor stub

}

UpdateJugador::~UpdateJugador() {
	// TODO Auto-generated destructor stub
}

void UpdateJugador::setRemitente(string remitente)
{
	this->remitente = remitente;
}

void UpdateJugador::setDestinatario(string destinatario)
{
	this->destinatario = destinatario;
}

void UpdateJugador::setX(string x)
{
	this->x = x;
}

void UpdateJugador::setY(string y)
{
	this->y = y;
}

void UpdateJugador::setCondicion(string condicion)
{
	this->condicion = condicion;
}

void UpdateJugador::setSpriteActual(SpriteDto* sprite)
{
	this->spriteActual = sprite;
}

void UpdateJugador::setSpriteAnterior(SpriteDto* sprite)
{
	this->spriteAnterior = sprite;
}

string UpdateJugador::getRemitente()
{
	return this->remitente;
}

string UpdateJugador::getDestinatario()
{
	return this->destinatario;
}

string UpdateJugador::getX()
{
	return this->x;
}

string UpdateJugador::getY()
{
	return this->y;
}

string UpdateJugador::getCondicion()
{
	return this->condicion;
}
SpriteDto* UpdateJugador::getSpriteActual()
{
	return this->spriteActual;
}

SpriteDto* UpdateJugador::getSpriteAnterior()
{
	return this->spriteAnterior;
}
