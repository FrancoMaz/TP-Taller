/*
 * SpriteDto.cpp
 *
 *  Created on: 9 oct. 2016
 *      Author: josefina
 */

#include "SpriteDto.h"

SpriteDto::SpriteDto(string id, string cantidadDeFotogramas, string ancho, string alto, string path, string zIndex) {
	this->id = id;
	this->cantidadDeFotogramas = cantidadDeFotogramas;
	this->ancho = ancho;
	this->alto = alto;
	this->path = path;
	this->zIndex = zIndex;
}

string SpriteDto::getId() {
	return this->id;
}

string SpriteDto::getCantidadDeFotogramas() {
	return this->cantidadDeFotogramas;
}

string SpriteDto::getAncho() {
	return this->ancho;
}

string SpriteDto::getAlto() {
	return this->alto;
}

string SpriteDto::getPath() {
	return this->path;
}

string SpriteDto::getZIndex() {
	return this->zIndex;
}

SpriteDto::~SpriteDto() {
	// TODO Auto-generated destructor stub
}

