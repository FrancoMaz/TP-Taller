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

const char* ImagenDto::getAncho() {
	return this->ancho;
}

const char* ImagenDto::getAlto() {
	return this->alto;
}

ImagenDto::~ImagenDto() {
	// TODO Auto-generated destructor stub
}
