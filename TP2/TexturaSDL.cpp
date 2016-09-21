/*
 * TexturaSDL.cpp
 *
 *  Created on: 21 sep. 2016
 *      Author: pablo
 */

#include "TexturaSDL.h"
#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

TexturaSDL::TexturaSDL(SDL_Renderer* renderer){
	this->textura = NULL;
	this->render = renderer;
	this->ancho = 0;
	this->alto = 0;
	this->posX = 0;
	this->posY = 0;
}

TexturaSDL::~TexturaSDL(){
	this->limpiar();
}

bool TexturaSDL::cargarImagen(string ruta){
	bool finalizado = true;

	this->limpiar();

	//Cargo la imagen de una ruta especificada
	SDL_Surface* imagenCargada = IMG_Load(ruta.c_str());
	if (imagenCargada == NULL){
		cout << "No se pudo cargar la imagen del archivo " << ruta.c_str() << endl;
		cout << "Error: " << IMG_GetError() << endl;
		finalizado = false;
	} else {
		//Colocamos el color al que vamos a tomarlo como transparente (tomo el color blanco)
		SDL_SetColorKey(imagenCargada,SDL_TRUE,SDL_MapRGB(imagenCargada->format,0xFF,0xFF,0xFF));
		//Creamos una textura a partir de los pixeles del surface/imagen cargada
		this->textura = SDL_CreateTextureFromSurface(this->render, imagenCargada);
		if (this->textura == NULL){
			cout << "No se pudo crear la textura del archivo " << ruta.c_str() << endl;
			cout << "Error: " << SDL_GetError() << endl;
			finalizado = false;
		} else {
			//Almacenamos las dimensiones de la imagen
			this->ancho = imagenCargada->w;
			this->alto = imagenCargada->h;
		}
		SDL_FreeSurface(imagenCargada);
	}

	return finalizado;
}

void TexturaSDL::limpiar(){
	//Libero las texturas si estas existen
	if (this->textura != NULL) {
		SDL_DestroyTexture(this->textura);
		this->textura = NULL;
		this->alto = 0;
		this->ancho = 0;
		this->posX = 0;
		this->posY = 0;
	}
}

void TexturaSDL::aplicarPosicion(float x, float y){
	SDL_Rect rectangulo = {x,y,this->ancho,this->alto};
	SDL_RenderCopy(this->render, this->textura, NULL, &rectangulo);
}

int TexturaSDL::getAncho(){
	return this->ancho;
}

int TexturaSDL::getAlto(){
	return this->alto;
}

float TexturaSDL::getPosX(){
	return this->posX;
}

float TexturaSDL::getPosY(){
	return this->posY;
}
