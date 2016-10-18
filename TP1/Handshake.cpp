/*
 * Handshake.cpp
 *
 *  Created on: 18 oct. 2016
 *      Author: franco
 */

#include "Handshake.h"

Handshake::Handshake(ImagenDto* imagen1, ImagenDto* imagen2, SetDeSpritesDto* setSprite1, SetDeSpritesDto* setSprite2, SetDeSpritesDto* setSprite3, char* ancho, char* alto) {
	this->imagen1 = imagen1;
	this->imagen2 = imagen2;
	this->setSprite1 = setSprite1;
	this->setSprite2 = setSprite2;
	this->setSprite3 = setSprite3;
	this->ancho = ancho;
	this->alto = alto;

}

Handshake::~Handshake() {
	// TODO Auto-generated destructor stub
}

void Handshake::setImagen1(ImagenDto* imagen){
	this->imagen1 = imagen;
}

void Handshake::setImagen2(ImagenDto* imagen){
	this->imagen2 = imagen;
}

void Handshake::setSprites1(SetDeSpritesDto* sprites){
	this->setSprite1 = sprites;
}

void Handshake::setSprites2(SetDeSpritesDto* sprites){
	this->setSprite2 = sprites;
}

void Handshake::setSprites3(SetDeSpritesDto* sprites){
	this->setSprite3 = sprites;
}

void Handshake::setAlto(char* alto){
	this->alto = alto;
}

void Handshake::setAncho(char* ancho){
	this->ancho = ancho;
}

SetDeSpritesDto* Handshake::getSprites1() {
	return setSprite1;
}

SetDeSpritesDto* Handshake::getSprites2() {
	return setSprite2;
}

SetDeSpritesDto* Handshake::getSprites3() {
	return setSprite3;
}

ImagenDto* Handshake::getImagen1() {
	return this->imagen1;
}

ImagenDto* Handshake::getImagen2() {
	return this->imagen2;
}
