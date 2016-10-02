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

	SDL_StartTextInput();

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
					renderizarTexto = true;
				} else {
					//Si presionamos Ctrl+C copiamos el contenido del texto en el portapapeles
					if(this->evento.key.keysym.sym == SDLK_c && (SDL_GetModState() & KMOD_CTRL)){
						//El (SDL_GetModState() & KMOD_CTRL) lo que hace es asegurarnos que mantengamos el Ctrl
						SDL_SetClipboardText(textoParaActualizar->c_str());
					} else {
						//Si presionamos Ctrl+V pegamos el contenido del portapapeles en el texto
						if(this->evento.key.keysym.sym == SDLK_v &&(SDL_GetModState() & KMOD_CTRL)){
							SDL_GetClipboardText();
							renderizarTexto = true;
						}
					}
				}
			} else {
				//Trabajamos con los eventos de entrada para el texto
				if(this->evento.type == SDL_TEXTINPUT){
					//Si no copiamos (presionamos Ctrl+c) o pegamos (presionamos Ctrl+v)
					if(!((this->evento.text.text[0] == 'c' || this->evento.text.text[0] == 'C')&&(this->evento.text.text[0] == 'v' || this->evento.text.text[0] == 'V'))&&(SDL_GetModState() & KMOD_CTRL)){
						//Asignamos el caracter al texto
						*textoParaActualizar += this->evento.text.text;
						renderizarTexto = true;
					}
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
