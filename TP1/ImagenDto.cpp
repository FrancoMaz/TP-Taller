/*
 * ImagenDto.cpp
 *
 *  Created on: 9 oct. 2016
 *      Author: josefina
 */

#include "ImagenDto.h"

ImagenDto::ImagenDto(const char* path, const char* zIndex, const char* velocidad) {
	this->path = path;
	this->zIndex = zIndex;
	this->velocidad = velocidad;
}

string ImagenDto::getPath() {
	return string(this->path);
}

const char* ImagenDto::getZIndex() {
	return this->zIndex;
}

const char* ImagenDto::getVelocidad() {
	return this->velocidad;
}

ImagenDto::~ImagenDto() {
	// TODO Auto-generated destructor stub
}
