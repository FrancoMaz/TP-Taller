/*
 * SpriteDto.cpp
 *
 *  Created on: 9 oct. 2016
 *      Author: josefina
 */

#include "SpriteDto.h"

SpriteDto::SpriteDto(const char* id, const char* cantidadDeFotogramas, const char* ancho, const char* alto, const char* path, const char* zIndex) {
	this->id = id;
	this->cantidadDeFotogramas = cantidadDeFotogramas;
	this->ancho = ancho;
	this->alto = alto;
	this->path = path;
	this->zIndex = zIndex;
}

SpriteDto::~SpriteDto() {
	// TODO Auto-generated destructor stub
}

