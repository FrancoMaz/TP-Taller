/*
 * Handshake.cpp
 *
 *  Created on: 18 oct. 2016
 *      Author: franco
 */

#include "Handshake.h"

Handshake::Handshake(vector<ImagenDto*> imagenes, vector<SetDeSpritesDto*> sprites, string ancho, string alto) {
	vector<SpriteDto*> vectorRojo;
	vector<SpriteDto*> vectorVerde;
	vector<SpriteDto*> vectorAmarillo;
	vector<SpriteDto*> vectorAzul;
	int contColor;
	vector<SpriteDto*> spritesJugadores = sprites.at(0)->getSprites();
	for (int i = 0; i < spritesJugadores.size(); i++)
	{
		contColor = i % 4;
		switch (contColor)
		{
			case 0:
			{
				vectorRojo.push_back(spritesJugadores.at(i));
				break;
			}
			case 1:
			{
				vectorVerde.push_back(spritesJugadores.at(i));
				break;
			}
			case 2:
			{
				vectorAmarillo.push_back(spritesJugadores.at(i));
				break;
			}
			case 3:
			{
				vectorAzul.push_back(spritesJugadores.at(i));
				break;
			}

		}
	}
	this->matrizSpritesJugadores.push_back(vectorRojo);
	this->matrizSpritesJugadores.push_back(vectorVerde);
	this->matrizSpritesJugadores.push_back(vectorAmarillo);
	this->matrizSpritesJugadores.push_back(vectorAzul);

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

vector<SetDeSpritesDto*> Handshake::getSprites()
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
