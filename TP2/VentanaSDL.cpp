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

VentanaSDL::VentanaSDL(){
	this->ventana = NULL;
	this->capaPrincipal = NULL;
	this->imagenDeEspera = NULL;
}

VentanaSDL::~VentanaSDL(){}

//Esta función crea la ventana principal del juego con sus resoluciones (si la función devuelve false, ocurrió un error)
bool VentanaSDL::inicializar(){
	bool finalizado = true;

	if(SDL_Init(SDL_INIT_VIDEO) != 0){
		cout << "SDL no pudo inicializarse. Error: " << SDL_GetError() << endl;
		finalizado = false;
	} else {
		this->ventana = SDL_CreateWindow("Metal Slug (Alpha Version: 0.001)", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ANCHO_VENTANA, ALTO_VENTANA, SDL_WINDOW_SHOWN);
		if (this->ventana == NULL){
			cout << "La ventana no pudo crearse. Error: " << SDL_GetError() << endl;
			finalizado = false;
		} else {
			this->capaPrincipal = SDL_GetWindowSurface(ventana);
		}
	}
	return finalizado;
}

//Esta función carga una imagen externa (de una ruta de archivo) a una variable de tipo surface (si la función devuelve false, ocurrió un error)
bool VentanaSDL::cargarImagen(string rutaDeArchivo){
	bool finalizado = true;

	SDL_Surface* imagenCargada = SDL_LoadBMP(rutaDeArchivo.c_str());

	if (imagenCargada == NULL){
		cout << "No se pudo cargar la imagen. Error: " << SDL_GetError() << endl;
		finalizado = false;
	} else {
		//Optimizamos el surface
		this->imagenDeEspera = SDL_ConvertSurface(imagenCargada, capaPrincipal->format, 0);
		if (this->imagenDeEspera == NULL){
			cout << "No se pudo optimizar la imagen. Error: " << SDL_GetError() << endl;
		} else {
			SDL_FreeSurface(imagenCargada);
			SDL_BlitSurface(this->imagenDeEspera,NULL,this->capaPrincipal,NULL);
		}
	}

	return finalizado;
}

void VentanaSDL::actualizar(){
	SDL_UpdateWindowSurface(this->ventana);
}

void VentanaSDL::cerrar(){
	SDL_DestroyWindow(ventana);
	ventana = NULL;

	SDL_Quit();
}