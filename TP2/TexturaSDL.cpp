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
#include <vector>

TexturaSDL::TexturaSDL(SDL_Renderer* renderer){
	this->textura = NULL;
	this->render = renderer;
	this->frameActual = 0;
	this->ancho = 0;
	this->alto = 0;
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

			//Inicializamos el blender (para transparencias)
			SDL_SetTextureBlendMode(this->textura,SDL_BLENDMODE_BLEND);
		}
		SDL_FreeSurface(imagenCargada);
	}

	return finalizado;
}

void TexturaSDL::setAlpha(Uint8 alpha){
	SDL_SetTextureAlphaMod(this->textura,alpha);
}

void TexturaSDL::limpiar(){
	//Libero las texturas si estas existen
	if (this->textura != NULL) {
		SDL_DestroyTexture(this->textura);
		this->textura = NULL;
		this->alto = 0;
		this->ancho = 0;
	}
}

void TexturaSDL::aplicarPosicionYTamanio(float x, float y, int ancho, int alto, double rotacion, SDL_RendererFlip flip){
	SDL_Rect rectangulo = {x,y,ancho,alto};
	SDL_Rect* clip = NULL;

	if (this->spriteClips.size() != 0){
		//El frameActual lo divido por 6 para reducir la velocidad de fotogramas
		clip = &this->spriteClips[this->frameActual/6];
		rectangulo.w = clip->w;
		rectangulo.h = clip->h;
		this->frameActual++;
		if((this->frameActual/6) >= this->spriteClips.size()){
			this->frameReset();
		}
	}


	SDL_RenderCopyEx(this->render, this->textura, clip, &rectangulo,rotacion,NULL,flip);
}

void TexturaSDL::generarSprite(int frames){
	if (frames >= 2){
		SDL_Rect clips[frames];
		int clipAncho = (this->ancho)/frames;

		for (int i = 0, x = 0; i < frames; i++, x=x+clipAncho){
			clips[i].x = x;
			clips[i].y = 0;
			clips[i].w = clipAncho;
			clips[i].h = this->alto;

		this->spriteClips.push_back(clips[i]);
		}
	}
}

void TexturaSDL::frameReset(){
	this->frameActual = 0;
}

void TexturaSDL::aplicarPosicion(float x, float y, double rotacion, SDL_RendererFlip flip){
	this->aplicarPosicionYTamanio(x,y,this->ancho,this->alto, rotacion, flip);
}

void TexturaSDL::aplicarPosicionConTamanio(float x, float y, int ancho, int alto){
	this->aplicarPosicionYTamanio(x,y,ancho,alto,0,SDL_FLIP_NONE);
}
