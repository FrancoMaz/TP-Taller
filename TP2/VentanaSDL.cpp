/*
 * VentanaSDL.cpp
 *
 *  Created on: 18 sep. 2016
 *      Author: pablo
 */

#include "VentanaSDL.h"
#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

VentanaSDL::VentanaSDL(){
	this->ventana = NULL;
	this->renderizacion = NULL;
}

VentanaSDL::~VentanaSDL(){}

//Esta función crea la ventana principal del juego con sus resoluciones (si la función devuelve false, ocurrió un error)
bool VentanaSDL::inicializar(){
	bool finalizado = true;

	if(SDL_Init(SDL_INIT_VIDEO) != 0){
		cout << "SDL no pudo inicializarse. Error: " << SDL_GetError() << endl;
		finalizado = false;
	} else {
		//Establesco el filtro de textura a lineal
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"1")){
			cout << "Advertencia: El filtro de textura lineal no está habilitado." << endl;
		}

		//Creamos la ventana
		this->ventana = SDL_CreateWindow("Metal Slug (Beta Version)", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ANCHO_VENTANA, ALTO_VENTANA, SDL_WINDOW_SHOWN);
		if (this->ventana == NULL){
			cout << "La ventana no pudo crearse. Error: " << SDL_GetError() << endl;
			finalizado = false;
		} else {
			//Creamos la renderización para la ventana y la sincronización vertical
			this->renderizacion = SDL_CreateRenderer(ventana, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if (this->renderizacion == NULL){
				cout << "La renderización no pudo ser creada. Error: " << SDL_GetError() << endl;
				finalizado = false;
			} else {
				//Inicializamos el color del renderizado
				SDL_SetRenderDrawColor(this->renderizacion, 0xFF, 0xFF, 0xFF, 0xFF);

				//Inicializamos SDL_Image para poder cargar archivos PNG
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags)){
					cout << "SDL_Image no pudo ser inicializado. Error: " << IMG_GetError() << endl;
					finalizado = false;
				}

				//Inicializamos SDL_TTF
				if (TTF_Init() == -1){
					cout << "SDL_TFF no pudo ser inicializado. Error: " << TTF_GetError() << endl;
					finalizado = false;
				}
			}
		}
	}
	return finalizado;
}

void VentanaSDL::limpiar(){
	//Limpiar pantalla
	SDL_SetRenderDrawColor(this->renderizacion, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(this->renderizacion);
}

void VentanaSDL::actualizar(){
	//Actualizar pantalla
	SDL_RenderPresent(this->renderizacion);
}

TexturaSDL* VentanaSDL::crearModelo(string ruta, int valor, int opcion){
	bool cargaExitosa;

	TexturaSDL* textura = new TexturaSDL(this->renderizacion);

	switch (opcion){
	case 0:
		textura->cargarImagen(ruta);
		textura->generarSprite(valor);
		break;
	case 1:
		textura->cargarTexto(ruta, valor);
		break;
	}

	return textura;
}

TexturaSDL* VentanaSDL::crearTextura(string ruta, int frames){
	return (this->crearModelo(ruta,frames,0));
}

TexturaSDL* VentanaSDL::crearBoton(string ruta){
	return (this->crearModelo(ruta,3,0));
}

TexturaSDL* VentanaSDL::crearTexto(string ruta, int tamanio){
	return (this->crearModelo(ruta,tamanio,1));
}

void VentanaSDL::cerrar(){
	//Destruyo la ventana y el renderizador
	SDL_DestroyRenderer(this->renderizacion);
	this->renderizacion = NULL;
	SDL_DestroyWindow(this->ventana);
	this->ventana = NULL;

	//Salimos del SDL
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}
