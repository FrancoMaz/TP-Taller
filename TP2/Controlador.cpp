/*
 * Controlador.cpp
 *
 *  Created on: 1 oct. 2016
 *      Author: pablo
 */

#include <SDL2/SDL.h>
#include "Controlador.h"

using namespace std;

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
	while(SDL_PollEvent(&this->evento) != 0){
		if(this->evento.type == SDL_QUIT){
			this->botonCerrarVentana = true;
		} else {
			if(this->evento.type == tipo){
				if(this->evento.key.keysym.sym == boton){
					seleccionado = true;
				}
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

bool Controlador::ingresarCaracteresATexto(string* textoParaActualizar){
	//Indicador para poder renderizar el texto que querramos actualizar
	bool renderizarTexto = false;

	while(SDL_PollEvent(&evento) != 0){
		//Si presionamos el boton X de cerrar ventana...
		if(evento.type == SDL_QUIT){
			this->botonCerrarVentana = true;
		} else {
			//Si realizamos un evento de presionar teclado...
			if(this->evento.type == SDL_KEYDOWN){
				//Si mantengo el boton backspace...
				if(this->evento.key.keysym.sym == SDLK_BACKSPACE && textoParaActualizar->length() > 0){
					//Borramos un caracter del texto
					textoParaActualizar->pop_back();
					if (textoParaActualizar->length() == 0){
						*textoParaActualizar = " ";
					}
					renderizarTexto = true;
				}
			} else {
				//Trabajamos con los eventos de entrada para el texto
				if(this->evento.type == SDL_TEXTINPUT){
					//Asignamos el caracter al texto
					if (*textoParaActualizar == " "){
						*textoParaActualizar = this->evento.text.text;
					} else {
						if (textoParaActualizar->length() < 13){
							*textoParaActualizar += this->evento.text.text;
						}
					}
					renderizarTexto = true;
				}
			}
		}
	}
	return renderizarTexto;
}

bool Controlador::comprobarCierreVentana(){
	return botonCerrarVentana;
}

SDL_Event Controlador::obtenerEvento(){
	return this->evento;
}
