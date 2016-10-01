/*
 * Vista.cpp
 *
 *  Created on: 30 sep. 2016
 *      Author: pablo
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

Vista::Vista(){
	this->ventana = new VentanaSDL();

	textura.push_back(ventana->crearTextura("Recursos/Menu_fondo.png",0));
	textura.push_back(ventana->crearTextura("Recursos/Menu_Metal_Slug.png",0));
	textura.push_back(ventana->crearTextura("Recursos/Efecto_luz.png",0));
	textura.push_back(ventana->crearBoton("Recursos/Boton_Salir.png"));
	textura.push_back(ventana->crearBoton("Recursos/Boton_Creditos.png"));
	textura.push_back(ventana->crearBoton("Recursos/Boton_Ingresar.png"));
	textura.push_back(ventana->crearTextura("Recursos/Efecto_luz.png",0));
	textura.push_back(ventana->crearTextura("Recursos/Efecto_luz.png",0));
	textura.push_back(ventana->crearTexto("Recursos/arial.ttf",14));
	textura.push_back(ventana->crearTexto("Recursos/arial.ttf",14));
	textura.push_back(ventana->crearTexto("Recursos/arial.ttf",13));
}


