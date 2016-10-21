/*
 * Handshake.cpp
 *
 *  Created on: 18 oct. 2016
 *      Author: franco
 */

#include "Handshake.h"

Handshake::Handshake(vector<ImagenDto*> imagenes, vector<SetDeSpritesDto*> sprites, char* ancho, char* alto) {
	this->imagenes = imagenes;
	this->sprites = sprites;
	this->ancho = ancho;
	this->alto = alto;

}

Handshake::Handshake(vector<ImagenDto*> imagenes, vector<SetDeSpritesDto*> sprites, char* ancho, char* alto, char* cantidadMaxima) {
	this->imagenes = imagenes;
	this->sprites = sprites;
	this->ancho = ancho;
	this->alto = alto;
	this->cantidadMaximaJugadores = cantidadMaxima;
}

Handshake::~Handshake() {
	// TODO Auto-generated destructor stub
}

vector<SetDeSpritesDto*> Handshake::getSprites()
{
	return this->sprites;
}

vector<ImagenDto*> Handshake::getImagenes()
{
	return this->imagenes;
}
char* Handshake::getAncho(){
	return this->ancho;
}
