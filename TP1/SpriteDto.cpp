/*
 * SpriteDto.cpp
 *
 *  Created on: 9 oct. 2016
 *      Author: josefina
 */

#include "SpriteDto.h"

SpriteDto::SpriteDto(string id, int cantidadDeFotogramas, int ancho, int alto, const char* path, const char* zIndex) {
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

int SpriteDto::getCantidadDeFotogramas() {
	return this->cantidadDeFotogramas;
}

int SpriteDto::getAncho() {
	return this->ancho;
}

int SpriteDto::getAlto() {
	return this->alto;
}

const char* SpriteDto::getPath() {
	return this->path;
}

const char* SpriteDto::getZIndex() {
	return this->zIndex;
}

SpriteDto::~SpriteDto() {
	// TODO Auto-generated destructor stub
}

