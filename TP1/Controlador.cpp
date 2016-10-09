/*
 * Controlador.cpp
 *
 *  Created on: 1 oct. 2016
 *      Author: pablo
 */

#include <SDL2/SDL.h>
#include "Controlador.h"

using namespace std;

SDL_Event evento;

Controlador::Controlador(){
	this->botonCerrarVentana = false;
	this->teclaEscaneada = SDL_GetKeyboardState(NULL);
}

Controlador::~Controlador(){}

bool Controlador::presionarBoton(SDL_Keycode boton){
	return this->aplicarEvento(boton,SDL_KEYDOWN);
}

bool Controlador::soltarBoton(SDL_Keycode boton){
	return this->aplicarEvento(boton,SDL_KEYUP);
}

bool Controlador::aplicarEvento(SDL_Keycode boton, Uint32 tipo){
	bool seleccionado = false;
	if(evento.type == SDL_QUIT){
		this->botonCerrarVentana = true;
	} else {
		if(evento.type == tipo && evento.key.repeat == 0){
			if(evento.key.keysym.sym == boton){
				seleccionado = true;
			}
		}
	}
	return seleccionado;
}

bool Controlador::escanearBoton(SDL_Scancode boton){
	return this->teclaEscaneada[boton];
}

void Controlador::botonCerrarVentanaSeleccionado(){
	while(SDL_PollEvent(&evento) != 0){
		if(evento.type == SDL_QUIT){
			this->botonCerrarVentana = true;
		}
	}
}

void Controlador::ingresarCaracteresATexto(string* textoParaActualizar){
	while(SDL_PollEvent(&evento) != 0){
		//Si presionamos el boton X de cerrar ventana...
		if(evento.type == SDL_QUIT){
			this->botonCerrarVentana = true;
		} else {
			//Si realizamos un evento de presionar teclado...
			if(evento.type == SDL_KEYDOWN){
				//Si mantengo el boton backspace...
				if(evento.key.keysym.sym == SDLK_BACKSPACE && textoParaActualizar->length() > 0){
					//Borramos un caracter del texto
					textoParaActualizar->pop_back();
					if (textoParaActualizar->length() == 0){
						*textoParaActualizar = " ";
					}
				}
			} else {
				//Trabajamos con los eventos de entrada para el texto
				if(evento.type == SDL_TEXTINPUT){
					//Asignamos el caracter al texto
					if (*textoParaActualizar == " "){
						*textoParaActualizar = evento.text.text;
					} else {
						if (textoParaActualizar->length() < 13){
							*textoParaActualizar += evento.text.text;
						}
					}
				}
			}
		}
	}
}

bool Controlador::comprobarCierreVentana(){
	return botonCerrarVentana;
}

void Controlador::setCerrarVentana(){
	this->botonCerrarVentana = true;
}
