/*
 * Vista.cpp
 *
 *  Created on: 30 sep. 2016
 *      Author: pablo
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "VentanaSDL.h"
#include "TexturaSDL.h"
#include "Controlador.h"
#include "Vista.h"
#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

Vista::Vista(){
	this->ventana = new VentanaSDL();
	this->controlador = new Controlador();
}

bool Vista::inicializar(){
	return this->ventana->inicializar();
}

void Vista::cargarArchivos(){
	textura.push_back(ventana->crearTextura("Recursos/Menu_fondo.png",0));				//0: texturaMenuFondo
	textura.push_back(ventana->crearTextura("Recursos/Menu_Metal_Slug.png",0));			//1: texturaMenuMetalSlug
	textura.push_back(ventana->crearTextura("Recursos/Menu_Presionar_Enter.png",0));	//2: texturaPresionarEnter
	textura.push_back(ventana->crearTextura("Recursos/Efecto_luz.png",0));				//3: texturaEfectoLuz
	textura.push_back(ventana->crearBoton("Recursos/Boton_Salir.png"));					//4: texturaBotonSalir
	textura.push_back(ventana->crearBoton("Recursos/Boton_Creditos.png"));				//5: texturaBotonCreditos
	textura.push_back(ventana->crearBoton("Recursos/Boton_Ingresar.png"));				//6: texturaBotonIngresar
	textura.push_back(ventana->crearTextura("Recursos/Efecto_luz.png",0));				//7: campoTextoUno
	textura.push_back(ventana->crearTextura("Recursos/Efecto_luz.png",0));				//8: campoTextoDos
	textura.push_back(ventana->crearTexto("Recursos/arial.ttf",14));					//9: textoPuerto
	textura.push_back(ventana->crearTexto("Recursos/arial.ttf",14));					//10: textoIP
	textura.push_back(ventana->crearTexto("Recursos/arial.ttf",13));					//11: textoTPTaller
	textura.push_back(ventana->crearTexto("Recursos/arial.ttf",19));					//12: entradaTextoIP
}

void Vista::cargarPrimeraPantalla(){
	for (float y=-290; y<40; y=y+5){
		ventana->limpiar();
		textura[0]->aplicarPosicion(0,0,0,SDL_FLIP_NONE);
		textura[1]->aplicarPosicion(180,y,0,SDL_FLIP_NONE);
		ventana->actualizar();
	}

	SDL_Color colorTexto = {255,255,255};
	textura[11]->actualizarTexto("Trabajo Práctico - Taller de Programación", colorTexto);

	for (float a= 255; a>0; a=a-3){
		ventana->limpiar();
		textura[0]->aplicarPosicion(0,0,0,SDL_FLIP_NONE);
		textura[1]->aplicarPosicion(180,40,0,SDL_FLIP_NONE);
		textura[2]->aplicarPosicion(200,430,0,SDL_FLIP_NONE);
		textura[11]->aplicarPosicion(284,560,0,SDL_FLIP_NONE);
		textura[11]->setAlpha(200);
		textura[3]->aplicarPosicionConTamanio(0,0,800,600);
		textura[3]->setAlpha(a);
		ventana->actualizar();
	}

		bool incrementa = true;
		int a = 255;
		//Mientras la ventana no se cierre pulsando X o no se presione el enter, hacer el loop
		while ((!controlador->comprobarCierreVentana())&&(!controlador->presionarBoton(SDLK_RETURN))){
			ventana->limpiar();
			textura[0]->aplicarPosicion(0,0,0,SDL_FLIP_NONE);
			textura[1]->aplicarPosicion(180,40,0,SDL_FLIP_NONE);
			textura[2]->aplicarPosicion(200,430,0,SDL_FLIP_NONE);
			if (a>=255)	incrementa = false;
			if (a<=0) incrementa = true;
			if (incrementa){
				a=a+3;
			} else {
				a=a-3;
			}
			textura[2]->setAlpha(a);
			textura[11]->aplicarPosicion(284,560,0,SDL_FLIP_NONE);
			textura[11]->setAlpha(200);
			ventana->actualizar();
		}
}

void Vista::cargarSegundaPantalla(){
	SDL_Color colorTexto = {255,255,255};
	SDL_Color colorTextoEntrada = {0,0,0};
	SDL_Event e;
	string numeroIP = "78";
	int a = 230;
	bool salir = false;

	//Inicializamos la entrada para textos
	SDL_StartTextInput();

	textura[9]->actualizarTexto("Ingrese el puerto:",colorTexto);
	textura[10]->actualizarTexto("Ingrese la IP del servidor:",colorTexto);

	while ((!this->controlador->comprobarCierreVentana())&&(!salir)){
		e = this->controlador->obtenerEvento();
		ventana->limpiar();
		textura[0]->aplicarPosicion(0,0,0,SDL_FLIP_NONE);
		textura[1]->aplicarPosicion(180,40,0,SDL_FLIP_NONE);

		textura[9]->aplicarPosicion(310,337,0,SDL_FLIP_NONE);

		textura[7]->cargarImagen("Recursos/Efecto_luz.png");
		textura[7]->aplicarPosicionConTamanio(310,360,185,28);

		textura[12]->actualizarTexto(numeroIP,colorTextoEntrada);
		textura[12]->aplicarPosicion(316,363,0,SDL_FLIP_NONE);
		this->controlador->ingresarCaracteresATexto(&numeroIP);
		//Ingreso de texto
		//if(this->controlador->ingresarCaracteresATexto(&numeroIP)){
			textura[12]->actualizarTexto(numeroIP,colorTextoEntrada);
			textura[12]->aplicarPosicion(316,363,0,SDL_FLIP_NONE);
		//}

		textura[10]->aplicarPosicion(310,402,0,SDL_FLIP_NONE);
		textura[8]->aplicarPosicionConTamanio(310,425,185,28);

		if (textura[4]->aplicarPosicionDeBoton(656,550,&e)){
			salir = true;
		}

		textura[5]->aplicarPosicionDeBoton(30,550,&e);
		textura[6]->aplicarPosicionDeBoton(338,480,&e);

		textura[11]->aplicarPosicion(284,560,0,SDL_FLIP_NONE);
		textura[11]->setAlpha(200);
		textura[3]->aplicarPosicionConTamanio(0,0,800,600);
		textura[3]->setAlpha(a);
		ventana->actualizar();

		//this->controlador->botonCerrarVentanaSeleccionado();

		a = a-5;
		if (a <= 0) a = 0;
	}
}

void Vista::cerrar(){
	this->ventana->cerrar();
}

