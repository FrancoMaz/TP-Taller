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
	textura.push_back(ventana->crearBoton("Recursos/Boton_CampoDeTexto.png"));			//7: campoTextoUno
	textura.push_back(ventana->crearBoton("Recursos/Boton_CampoDeTexto.png"));			//8: campoTextoDos
	textura.push_back(ventana->crearTexto("Recursos/arial.ttf",14));					//9: textoIngresePuerto
	textura.push_back(ventana->crearTexto("Recursos/arial.ttf",14));					//10: textoIngreseIP
	textura.push_back(ventana->crearTexto("Recursos/msserif_bold.ttf",13));				//11: textoTPTaller
	textura.push_back(ventana->crearTexto("Recursos/arial.ttf",19));					//12: entradaTextoPuerto
	textura.push_back(ventana->crearTexto("Recursos/arial.ttf",19));					//13: entradaTextoIP
	textura.push_back(ventana->crearTexto("Recursos/msserif_bold.ttf",13));				//14: textoDatosNoCoinciden
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
		textura[11]->aplicarPosicion(254,560,0,SDL_FLIP_NONE);
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
		textura[11]->aplicarPosicion(254,560,0,SDL_FLIP_NONE);
		textura[11]->setAlpha(200);
		ventana->actualizar();
	}
}

void Vista::cargarSegundaPantalla(){
	SDL_Color colorTexto = {255,255,255};
	SDL_Color colorTextoAmarillo = {227,215,117};
	SDL_Color colorTextoEntrada = {0,0,0};
	SDL_Event e;
	string puerto = " ";
	string numeroIP = " ";
	int a = 230;
	bool campoUnoSeleccionado = false;
	bool campoDosSeleccionado = false;
	bool datosIncorrectos = false;
	bool siguiente = false;

	//Inicializamos la entrada para textos
	SDL_StartTextInput();

	textura[9]->actualizarTexto("Ingrese el puerto:",colorTexto);
	textura[10]->actualizarTexto("Ingrese la IP del servidor:",colorTexto);
	textura[14]->actualizarTexto("La dirección de ip o el puerto no permiten esta conexión",colorTextoAmarillo);

	textura[12]->actualizarTexto(puerto,colorTextoEntrada);
	textura[13]->actualizarTexto(numeroIP,colorTextoEntrada);

	while ((!this->controlador->comprobarCierreVentana())&&(!siguiente)){
		e = this->controlador->obtenerEvento();
		ventana->limpiar();
		textura[0]->aplicarPosicion(0,0,0,SDL_FLIP_NONE);
		textura[1]->aplicarPosicion(180,40,0,SDL_FLIP_NONE);

		textura[9]->aplicarPosicion(312,337,0,SDL_FLIP_NONE);
		textura[10]->aplicarPosicion(312,402,0,SDL_FLIP_NONE);

		if(textura[7]->aplicarPosicionDeBoton(310,358,&e)){
			campoUnoSeleccionado = true;
			campoDosSeleccionado = false;
			datosIncorrectos = false;
		}

		if (campoUnoSeleccionado){
			this->controlador->ingresarCaracteresATexto(&puerto);
			textura[7]->aplicarPosicionDeFrame(310,358,2,0,SDL_FLIP_NONE);
			textura[12]->actualizarTexto(puerto,colorTextoEntrada);
		}

		if(textura[8]->aplicarPosicionDeBoton(310,423,&e)){
			campoUnoSeleccionado = false;
			campoDosSeleccionado = true;
			datosIncorrectos = false;
		}

		if (campoDosSeleccionado){
			this->controlador->ingresarCaracteresATexto(&numeroIP);
			textura[8]->aplicarPosicionDeFrame(310,423,2,0,SDL_FLIP_NONE);
			textura[13]->actualizarTexto(numeroIP,colorTextoEntrada);
		}

		textura[12]->aplicarPosicion(316,363,0,SDL_FLIP_NONE);
		textura[13]->aplicarPosicion(316,428,0,SDL_FLIP_NONE);

		if (textura[4]->aplicarPosicionDeBoton(656,550,&e)){
			this->controlador->setCerrarVentana();
		}

		textura[5]->aplicarPosicionDeBoton(30,550,&e);

		if(textura[6]->aplicarPosicionDeBoton(338,475,&e)){
			campoUnoSeleccionado = false;
			campoDosSeleccionado = false;
			if((puerto == "7891")&&(numeroIP == "127.0.0.1")){
				siguiente = true;
				datosIncorrectos = false;
			} else {
				datosIncorrectos = true;
			}
		}

		if(datosIncorrectos){
			textura[14]->aplicarPosicion(205,530,0,SDL_FLIP_NONE);
		}

		textura[11]->aplicarPosicion(254,560,0,SDL_FLIP_NONE);
		textura[11]->setAlpha(200);
		textura[3]->aplicarPosicionConTamanio(0,0,800,600);
		textura[3]->setAlpha(a);
		ventana->actualizar();

		this->controlador->botonCerrarVentanaSeleccionado();

		a = a-5;
		if (a <= 0) a = 0;
	}
	SDL_StopTextInput();

	int angulo = 0;
	int x = 0;
	int y = 0;
	if (!this->controlador->comprobarCierreVentana()){
		for (float a= 0; a<255; a = a+5){
			ventana->limpiar();
			textura[0]->aplicarPosicion(0,0,0,SDL_FLIP_NONE);
			textura[1]->aplicarPosicion(180,40,0,SDL_FLIP_NONE);
			textura[9]->aplicarPosicion(312,337,0,SDL_FLIP_NONE);
			textura[10]->aplicarPosicion(312,402,0,SDL_FLIP_NONE);
			textura[7]->aplicarPosicionDeFrame(310-x,358-y,0,-angulo,SDL_FLIP_NONE);
			textura[8]->aplicarPosicionDeFrame(310+x,423-y,0,angulo,SDL_FLIP_NONE);
			textura[12]->aplicarPosicion(380-x,368-y,-angulo,SDL_FLIP_NONE);
			textura[13]->aplicarPosicion(360+x,433-y,angulo,SDL_FLIP_NONE);
			textura[4]->aplicarPosicionDeFrame(656,550,0,0,SDL_FLIP_NONE);
			textura[5]->aplicarPosicionDeFrame(30,550,0,0,SDL_FLIP_NONE);
			textura[6]->aplicarPosicionDeFrame(338,475,0,0,SDL_FLIP_NONE);
			textura[11]->aplicarPosicion(254,560,0,SDL_FLIP_NONE);
			textura[11]->setAlpha(200);
			textura[3]->aplicarPosicionConTamanio(0,0,800,600);
			textura[3]->setAlpha(a);
			angulo = angulo + 5;
			x = x+5;
			y = y+2;
			ventana->actualizar();
		}
	}
}

void Vista::cerrar(){
	this->ventana->cerrar();
}

