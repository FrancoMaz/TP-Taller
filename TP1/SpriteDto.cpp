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

string SpriteDto::getID() {
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

void SpriteDto::setID(string ID)
{
	this->id = ID;
}

void SpriteDto::setAncho(string ancho)
{
	this->ancho = ancho;
}

void SpriteDto::setAlto(string alto)
{
	this->alto = alto;
}

void SpriteDto::setZIndex(string zIndex)
{
	this->zIndex = zIndex;
}

void SpriteDto::setCantFotogramas(string cantFotogramas)
{
	this->cantidadDeFotogramas = cantFotogramas;
}

void SpriteDto::setPath(string path)
{
	this->path = path;
}

SpriteDto::~SpriteDto() {
	// TODO Auto-generated destructor stub
}
