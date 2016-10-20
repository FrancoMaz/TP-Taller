/*
 * ImagenDto.cpp
 *
 *  Created on: 9 oct. 2016
 *      Author: josefina
 */

#include "ImagenDto.h"

ImagenDto::ImagenDto(const char* path, const char* ancho, const char* alto) {
	this->path = path;
	this->ancho = ancho;
	this->alto = alto;
}

string ImagenDto::getPath() {
	return string(this->path);
}

int ImagenDto::getAncho() {
	return atoi(this->ancho);
}

int ImagenDto::getAlto() {
	return atoi(this->alto);
}

ImagenDto::~ImagenDto() {
	// TODO Auto-generated destructor stub
}
