/*
 * Handshake.cpp
 *
 *  Created on: 18 oct. 2016
 *      Author: franco
 */

#include "Handshake.h"

Handshake::Handshake(vector<ImagenDto*> imagenes, vector<SetDeSpritesDto*> sprites, string ancho, string alto) {
	this->imagenes = imagenes;
	this->sprites = sprites;
	this->ancho = ancho;
	this->alto = alto;

}

Handshake::Handshake(vector<ImagenDto*> imagenes, vector<SetDeSpritesDto*> sprites, string ancho, string alto, string cantidadMaxima) {
	this->imagenes = imagenes;
	this->sprites = sprites;
	this->ancho = ancho;
	this->alto = alto;
	this->cantidadMaximaJugadores = cantidadMaxima;
}

Handshake::~Handshake() {
	// TODO Auto-generated destructor stub
}

vector<SetDeSpritesDto*> Handshake::getSetSprites()
{
	return this->sprites;
}

vector<ImagenDto*> Handshake::getImagenes()
{
	return this->imagenes;
}
string Handshake::getAncho(){
	return this->ancho;
}

string Handshake::getAlto(){
	return this->alto;
}

string Handshake::getCantidadMaximaJugadores()
{
	return this->cantidadMaximaJugadores;
}
