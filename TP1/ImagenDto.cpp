/*
 * ImagenDto.cpp
 *
 *  Created on: 9 oct. 2016
 *      Author: josefina
 */

#include "ImagenDto.h"

ImagenDto::ImagenDto(string path, string ancho, string alto) {
	this->ID = path;
	this->ancho = ancho;
	this->alto = alto;
}

string ImagenDto::getID() {
	return this->ID;
}

string ImagenDto::getAncho() {
	return this->ancho;
}

string ImagenDto::getAlto() {
	return this->alto;
}

string ImagenDto::getZIndex()
{
	return this->zIndex;
}

void ImagenDto::setID(string ID)
{
	this->ID = ID;
}

void ImagenDto::setAncho(string ancho)
{
	this->ancho = ancho;
}

void ImagenDto::setAlto(string alto)
{
	this->alto = alto;
}

void ImagenDto::setZIndex(string zIndex)
{
	this->zIndex = zIndex;
}

ImagenDto::~ImagenDto() {
	// TODO Auto-generated destructor stub
}
