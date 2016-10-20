/*
 * ImagenDto.cpp
 *
 *  Created on: 9 oct. 2016
 *      Author: josefina
 */

#include "ImagenDto.h"

ImagenDto::ImagenDto(string path, string ancho, string alto) {
	this->path = path;
	this->ancho = ancho;
	this->alto = alto;
}

string ImagenDto::getPath() {
	return this->path;
}

string ImagenDto::getAncho() {
	return this->ancho;
}

string ImagenDto::getAlto() {
	return this->alto;
}

ImagenDto::~ImagenDto() {
	// TODO Auto-generated destructor stub
}
