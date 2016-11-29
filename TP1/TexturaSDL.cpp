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
	this->fuente = NULL;
	this->frameActual = 0;
	this->ancho = 0;
	this->alto = 0;
	this->velocidadFrameDisminuida = 2;
}

TexturaSDL::~TexturaSDL(){
	this->limpiarTextura();
	this->limpiarFuente();
}

void TexturaSDL::cargarImagen(string ruta){
	bool finalizado = true;

	this->limpiarTextura();
	this->limpiarFuente();

	//Cargo la imagen de una ruta especificada
	SDL_Surface* imagenCargada = IMG_Load(ruta.c_str());
	if (imagenCargada == NULL){
		cout << "No se pudo cargar la imagen del archivo " << ruta.c_str() << endl;
		cout << "Error: " << IMG_GetError() << endl;
		finalizado = false;
	} else {
		//Colocamos el color al que vamos a tomarlo como transparente (tomo el color blanco)
		SDL_SetColorKey(imagenCargada,SDL_TRUE,SDL_MapRGB(imagenCargada->format,254,254,0));
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

	//Si no se encuentra la ruta de imagen, cargamos una por defecto
	if (!finalizado){
		this->cargarImagenNoEncontrada();
	}
}

void TexturaSDL::cargarTexto(string ruta, int tamanio){
	this->limpiarTextura();
	this->limpiarFuente();
	bool finalizado = true;

	//Abrimos la fuente
	this->fuente = TTF_OpenFont(ruta.c_str(),tamanio);
	if (this->fuente == NULL){
		cout << "No se pudo cargar la fuente de texto del archivo " << ruta.c_str() << endl;
		cout << "Error: " << TTF_GetError() << endl;
		finalizado = false;
	} else {
		//Renderizo el texto
		SDL_Color colorTexto = {255,255,255};		//RGB (Rojo=0,Verde=0,Azul=0)
		if(!this->actualizarTexto(" ",colorTexto)){
			cout << "Error al renderizar la textura del texto" << endl;
			finalizado = false;
		}
	}

	//Si no se encuentra la ruta de imagen, cargamos una por defecto
	if (!finalizado){
		cout << "Se cargará una fuente por defecto: Arial" << endl;
		this->cargarTextoPorDefecto(tamanio);
	}
}

bool TexturaSDL::actualizarTexto(string texto, SDL_Color color){
	this->limpiarTextura();
	//Renderizamos el texto pasado por argumento
	SDL_Surface* textoCargado = TTF_RenderUTF8_Solid(this->fuente,texto.c_str(),color);
	if (textoCargado == NULL){
		cout << "No se pudo renderizar el texto" << endl;
		cout << "Error: " << IMG_GetError() << endl;
	} else {
		//Creo la textura desde el surface creado arriba
		this->textura = SDL_CreateTextureFromSurface(render,textoCargado);
		if (this->textura == NULL){
			cout << "No se pudo crear la textura del texto renderizado" << endl;
			cout << "Error: " << SDL_GetError() << endl;
		} else {
			//Obtengo las dimensiones
			this->ancho = textoCargado->w;
			this->alto = textoCargado->h;
		}
		SDL_FreeSurface(textoCargado);
	}

	return (this->textura != NULL);
}

void TexturaSDL::setAlpha(Uint8 alpha){
	SDL_SetTextureAlphaMod(this->textura,alpha);
}

void TexturaSDL::limpiarTextura(){
	//Libero las texturas si estas existen
	if (this->textura != NULL) {
		SDL_DestroyTexture(this->textura);
		this->textura = NULL;
		this->alto = 0;
		this->ancho = 0;
	}
}

void TexturaSDL::limpiarFuente(){
	//Libero la fuente
	if (this->fuente != NULL){
		TTF_CloseFont(this->fuente);
		this->fuente = NULL;
	}
}

void TexturaSDL::aplicarPosicionYTamanio(float x, float y, int ancho, int alto, SDL_Rect* clip, double rotacion, SDL_RendererFlip flip){
	SDL_Rect rectangulo = {(int)x,(int)y,ancho,alto};

	if (this->spriteClips.size() != 0){
		//El frameActual lo divido por 8 para reducir la velocidad de fotogramas
		clip = &this->spriteClips[this->frameActual/this->velocidadFrameDisminuida];
		rectangulo.w = clip->w;
		rectangulo.h = clip->h;
		this->frameActual++;
		if((this->frameActual/this->velocidadFrameDisminuida) >= this->spriteClips.size()){
			this->frameReset();
		}
	}

	if (clip != NULL){
		rectangulo.w = clip->w;
		rectangulo.h = clip->h;
	}

	SDL_RenderCopyEx(this->render, this->textura, clip, &rectangulo,rotacion,NULL,flip);
}

void TexturaSDL::generarSprite(int frames){
	//this->frameActual = 0;
	if (frames >= 2){
		this->spriteClips.clear();
		SDL_Rect clips[frames];
		int clipAncho = (this->ancho)/frames;

		for (int i = 0, x = 0; i < frames; i++, x=x+clipAncho){
			clips[i].x = x;
			clips[i].y = 0;
			clips[i].w = clipAncho;
			clips[i].h = this->alto;

		this->spriteClips.push_back(clips[i]);
		}
	} else {
		this->spriteClips.clear();
	}
}

void TexturaSDL::frameReset(){
	this->frameActual = 0;
}

void TexturaSDL::cargarImagenNoEncontrada(){
	this->cargarImagen("Recursos/ImagenNoEncontrada.png");
}

void TexturaSDL::cargarTextoPorDefecto(int tamanio){
	this->cargarTexto("Recursos/arial.ttf",tamanio);
}

void TexturaSDL::aplicarPosicion(float x, float y, double rotacion, SDL_RendererFlip flip){
	this->aplicarPosicionYTamanio(x,y,this->ancho,this->alto, NULL, rotacion, flip);
}

bool TexturaSDL::aplicarPosicionDeBoton(float x, float y, SDL_Event* e){
	bool botonClickeado = false;
	//Si ocurre algún evento del mouse
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP){
		//Obtengo la posición del mouse
		int mouse_x,mouse_y;
		SDL_GetMouseState(&mouse_x,&mouse_y);

		//Luego compruebo si la flecha del mouse está en el botón
		bool estaDentro = true;

		//Si la posición del mouse está a la izquierda del boton...
		if (mouse_x < x){
			estaDentro = false;
		} else {
			//Si la posición del mouse está a la derecha del boton...
			if (mouse_x > x + this->ancho/3){
				estaDentro = false;
			} else {
				//Si la posición del mouse está arriba del boton...
				if (mouse_y < y){
					estaDentro = false;
				} else {
					//Si la posición del mouse está debajo del boton...
					if (mouse_y > y + this->alto){
						estaDentro = false;
					}
				}
			}
		}

		//Si el mouse está fuera del botón...
		if (!estaDentro){
			this->aplicarPosicionDeFrame(x,y,0,0,SDL_FLIP_NONE);
		} else {
			//Si el mouse está dentro del boton, colocamos las texturas correspondientes
			switch(e->type){
				case SDL_MOUSEMOTION:
				this->aplicarPosicionDeFrame(x,y,1,0,SDL_FLIP_NONE);
				break;

				case SDL_MOUSEBUTTONDOWN:
				this->aplicarPosicionDeFrame(x,y,2,0,SDL_FLIP_NONE);
				break;

				case SDL_MOUSEBUTTONUP:
				this->aplicarPosicionDeFrame(x,y,1,0,SDL_FLIP_NONE);
				botonClickeado = true;
				break;
			}
		}
	} else {
		this->aplicarPosicionDeFrame(x,y,0,0,SDL_FLIP_NONE);
	}
	return botonClickeado;
}

void TexturaSDL::aplicarPosicionConTamanio(float x, float y, int ancho, int alto){
	this->aplicarPosicionYTamanio(x,y,ancho,alto,NULL,0,SDL_FLIP_NONE);
}

void TexturaSDL::aplicarPosicionDeFrame(float x, float y, int frame, double rotacion, SDL_RendererFlip flip){
	if(frame < this->spriteClips.size()){
		this->frameActual = frame*(this->velocidadFrameDisminuida);
		this->aplicarPosicionYTamanio(x,y,this->ancho,this->alto, NULL, rotacion, flip);
	}
}

void TexturaSDL::aplicarPosicionDePorcion(float x, float y, SDL_Rect* clip, double rotacion, SDL_RendererFlip flip){
	this->aplicarPosicionYTamanio(x,y,this->ancho,this->alto, clip, rotacion, flip);
}

int TexturaSDL::getAncho(){
	return this->ancho;
}

int TexturaSDL::getAlto(){
	return this->alto;
}

int TexturaSDL::getAnchoSprite(){
	if (this->spriteClips.size() != 0){
		return (this->ancho/this->spriteClips.size());
	} else {
		return this->ancho;
	}
}

int TexturaSDL::getAltoSprite(){
	if (this->spriteClips.size() != 0){
		return (this->alto/this->spriteClips.size());
	} else {
		return this->alto;
	}
}

void TexturaSDL::setVelocidadDisminuida(int valor){
	this->velocidadFrameDisminuida = valor;
}
