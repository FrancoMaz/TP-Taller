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

VentanaSDL::VentanaSDL(){
	this->ventana = NULL;
	this->renderizacion = NULL;
	this->texturasCargadas = true;
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
		this->ventana = SDL_CreateWindow("Metal Slug (Alpha Version: 0.06)", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ANCHO_VENTANA, ALTO_VENTANA, SDL_WINDOW_SHOWN);
		if (this->ventana == NULL){
			cout << "La ventana no pudo crearse. Error: " << SDL_GetError() << endl;
			finalizado = false;
		} else {
			//Creamos la renderización para la ventana
			this->renderizacion = SDL_CreateRenderer(ventana, -1, SDL_RENDERER_ACCELERATED);
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

TexturaSDL* VentanaSDL::crearTextura(string ruta){
	bool cargaExitosa;

	TexturaSDL* textura = new TexturaSDL(this->renderizacion);
	cargaExitosa = textura->cargarImagen(ruta);

	if (this->texturasCargadas){
		this->texturasCargadas = cargaExitosa;
	}

	return textura;
}

bool VentanaSDL::comprobarTexturasCargadas(){
	return texturasCargadas;
}

void VentanaSDL::cerrar(){
	//Destruyo la ventana y el renderizador
	SDL_DestroyRenderer(this->renderizacion);
	this->renderizacion = NULL;
	SDL_DestroyWindow(this->ventana);
	this->ventana = NULL;

	//Salimos del SDL
	IMG_Quit();
	SDL_Quit();
}
