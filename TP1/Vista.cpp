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
	this->opacidad = 255;

	this->datos.puerto = " ";
	this->datos.ip = " ";
	this->datos.nombre = " ";
	this->datos.contrasenia = " ";
}

bool Vista::inicializar(){
	return this->ventana->inicializar();
}

void Vista::cargarArchivos(){
	textura.push_back(ventana->crearTextura("Recursos/Menu_fondo.png",0));
	textura.push_back(ventana->crearTextura("Recursos/Menu_Metal_Slug.png",0));
	textura.push_back(ventana->crearTextura("Recursos/Menu_Presionar_Enter.png",0));
	textura.push_back(ventana->crearTextura("Recursos/Efecto_luz.png",0));
	textura.push_back(ventana->crearBoton("Recursos/Boton_Salir.png"));
	textura.push_back(ventana->crearBoton("Recursos/Boton_Creditos.png"));
	textura.push_back(ventana->crearBoton("Recursos/Boton_Ingresar.png"));
	textura.push_back(ventana->crearBoton("Recursos/Boton_CampoDeTexto.png"));
	textura.push_back(ventana->crearBoton("Recursos/Boton_CampoDeTexto.png"));
	textura.push_back(ventana->crearTexto("Recursos/arial.ttf",14));
	textura.push_back(ventana->crearTexto("Recursos/arial.ttf",14));
	textura.push_back(ventana->crearTexto("Recursos/msserif_bold.ttf",13));
	textura.push_back(ventana->crearTexto("Recursos/arial.ttf",19));
	textura.push_back(ventana->crearTexto("Recursos/arial.ttf",19));
	textura.push_back(ventana->crearTexto("Recursos/msserif_bold.ttf",13));
	textura.push_back(ventana->crearBoton("Recursos/Boton_Conectar.png"));

	//Defino constantes para cada textura (para evitar llamarlos por índices)
	#define texturaMenuFondo textura[0]
	#define texturaMenuMetalSlug textura[1]
	#define texturaPresionarEnter textura[2]
	#define texturaEfectoLuz textura[3]
	#define texturaBotonSalir textura[4]
	#define texturaBotonCreditos textura[5]
	#define texturaBotonIngresar textura[6]
	#define campoTextoUno textura[7]
	#define campoTextoDos textura[8]
	#define textoIngresePuerto textura[9]
	#define textoIngreseIP textura[10]
	#define textoTPTaller textura[11]
	#define entradaTextoPuerto textura[12]
	#define entradaTextoIP textura[13]
	#define textoDatosNoCoinciden textura[14]
	#define texturaBotonConectar textura[15]
}

void Vista::cargarPrimeraPantalla(){
	for (float y=-290; y<40; y=y+5){
		ventana->limpiar();
		texturaMenuFondo->aplicarPosicion(0,0,0,SDL_FLIP_NONE);
		texturaMenuMetalSlug->aplicarPosicion(180,y,0,SDL_FLIP_NONE);
		ventana->actualizar();
	}

	SDL_Color colorTexto = {255,255,255};
	textura[11]->actualizarTexto("Trabajo Práctico - Taller de Programación", colorTexto);

	for (this->opacidad; this->opacidad>0; this->opacidad=this->opacidad-3){
		ventana->limpiar();
		texturaMenuFondo->aplicarPosicion(0,0,0,SDL_FLIP_NONE);
		texturaMenuMetalSlug->aplicarPosicion(180,40,0,SDL_FLIP_NONE);
		texturaPresionarEnter->aplicarPosicion(200,430,0,SDL_FLIP_NONE);
		textoTPTaller->aplicarPosicion(254,560,0,SDL_FLIP_NONE);
		textoTPTaller->setAlpha(200);
		texturaEfectoLuz->aplicarPosicionConTamanio(0,0,800,600);
		texturaEfectoLuz->setAlpha(this->opacidad);
		ventana->actualizar();
	}

	bool incrementa = true;
	this->opacidad = 255;
	//Mientras la ventana no se cierre pulsando X o no se presione el enter, hacer el loop
	while ((!controlador->comprobarCierreVentana())&&(!controlador->presionarBoton(SDLK_RETURN))){
		ventana->limpiar();
		texturaMenuFondo->aplicarPosicion(0,0,0,SDL_FLIP_NONE);
		texturaMenuMetalSlug->aplicarPosicion(180,40,0,SDL_FLIP_NONE);
		texturaPresionarEnter->aplicarPosicion(200,430,0,SDL_FLIP_NONE);
		if (this->opacidad>=255)	incrementa = false;
		if (this->opacidad<=0) incrementa = true;
		if (incrementa){
			this->opacidad=this->opacidad+3;
		} else {
			this->opacidad=this->opacidad-3;
		}
		texturaPresionarEnter->setAlpha(this->opacidad);
		textoTPTaller->aplicarPosicion(254,560,0,SDL_FLIP_NONE);
		textoTPTaller->setAlpha(200);
		ventana->actualizar();
	}
	this->opacidad = 230;
}

datosConexion Vista::cargarSegundaPantalla(bool aviso){
	return cargarPantallaIngresoDatos(aviso,2);
}

datosConexion Vista::cargarTerceraPantalla(bool aviso){
	return cargarPantallaIngresoDatos(aviso,3);
}

datosConexion Vista::cargarPantallaIngresoDatos(bool aviso, int numeroPantalla){
	SDL_Color colorTexto = {255,255,255};
	SDL_Color colorTextoAmarillo = {227,215,117};
	SDL_Color colorTextoEntrada = {0,0,0};
	SDL_Event e;
	string campoUno;
	string campoDos;
	bool campoUnoSeleccionado = false;
	bool campoDosSeleccionado = false;
	bool siguiente = false;

	//Inicializamos la entrada para textos
	SDL_StartTextInput();

	switch(numeroPantalla){
		case 2:
			campoUno = this->datos.puerto;
			campoDos = this->datos.ip;
			textoIngresePuerto->actualizarTexto("Ingrese el puerto:",colorTexto);
			textoIngreseIP->actualizarTexto("Ingrese la IP del servidor:",colorTexto);
			textoDatosNoCoinciden->actualizarTexto("La dirección de ip o el puerto no permiten esta conexión",colorTextoAmarillo);
			break;
		case 3:
			campoUno = this->datos.nombre;
			campoDos = this->datos.contrasenia;
			textoIngresePuerto->actualizarTexto("Ingrese el nombre:",colorTexto);
			textoIngreseIP->actualizarTexto("Ingrese la contraseña:",colorTexto);
			textoDatosNoCoinciden->actualizarTexto("Usuario/contraseña incorrectos, inténtelo de nuevo",colorTextoAmarillo);
			break;
	}

	textura[12]->actualizarTexto(campoUno,colorTextoEntrada);
	textura[13]->actualizarTexto(campoDos,colorTextoEntrada);

	while ((!this->controlador->comprobarCierreVentana())&&(!siguiente)){
		e = this->controlador->obtenerEvento();
		ventana->limpiar();
		texturaMenuFondo->aplicarPosicion(0,0,0,SDL_FLIP_NONE);
		texturaMenuMetalSlug->aplicarPosicion(180,40,0,SDL_FLIP_NONE);

		textoIngresePuerto->aplicarPosicion(312,337,0,SDL_FLIP_NONE);
		textoIngreseIP->aplicarPosicion(312,402,0,SDL_FLIP_NONE);

		if(campoTextoUno->aplicarPosicionDeBoton(310,358,&e)){
			campoUnoSeleccionado = true;
			campoDosSeleccionado = false;
			aviso = false;
		}

		if (campoUnoSeleccionado){
			this->controlador->ingresarCaracteresATexto(&campoUno);
			campoTextoUno->aplicarPosicionDeFrame(310,358,2,0,SDL_FLIP_NONE);
			entradaTextoPuerto->actualizarTexto(campoUno,colorTextoEntrada);
		}

		if(campoTextoDos->aplicarPosicionDeBoton(310,423,&e)){
			campoUnoSeleccionado = false;
			campoDosSeleccionado = true;
			aviso = false;
		}

		if (campoDosSeleccionado){
			this->controlador->ingresarCaracteresATexto(&campoDos);
			campoTextoDos->aplicarPosicionDeFrame(310,423,2,0,SDL_FLIP_NONE);
			entradaTextoIP->actualizarTexto(campoDos,colorTextoEntrada);
		}

		entradaTextoPuerto->aplicarPosicion(316,363,0,SDL_FLIP_NONE);
		entradaTextoIP->aplicarPosicion(316,428,0,SDL_FLIP_NONE);

		if (texturaBotonSalir->aplicarPosicionDeBoton(656,550,&e)){
			this->controlador->setCerrarVentana();
		}

		texturaBotonCreditos->aplicarPosicionDeBoton(30,550,&e);

		switch(numeroPantalla){
			case 2:
				if(texturaBotonIngresar->aplicarPosicionDeBoton(338,475,&e)){
					campoUnoSeleccionado = false;
					campoDosSeleccionado = false;
					this->datos.puerto = campoUno;
					this->datos.ip = campoDos;
					siguiente = true;
				}
				break;
			case 3:
				if(texturaBotonConectar->aplicarPosicionDeBoton(338,475,&e)){
					campoUnoSeleccionado = false;
					campoDosSeleccionado = false;
					this->datos.nombre = campoUno;
					this->datos.contrasenia = campoDos;
					siguiente = true;
				}
				break;
		}

		if((aviso)&&((campoUno != " ")||(campoDos != " "))){
			switch(numeroPantalla){
				case 2:
					textoDatosNoCoinciden->aplicarPosicion(205,530,0,SDL_FLIP_NONE);
					break;
				case 3:
					textoDatosNoCoinciden->aplicarPosicion(215,530,0,SDL_FLIP_NONE);
					break;
			}
		}

		textoTPTaller->aplicarPosicion(254,560,0,SDL_FLIP_NONE);
		textoTPTaller->setAlpha(200);
		texturaEfectoLuz->aplicarPosicionConTamanio(0,0,800,600);
		texturaEfectoLuz->setAlpha(this->opacidad);
		ventana->actualizar();

		this->controlador->botonCerrarVentanaSeleccionado();

		this->opacidad = this->opacidad-5;
		if (this->opacidad <= 0) this->opacidad = 0;
	}
	SDL_StopTextInput();
	return this->datos;
}

void Vista::transicionDePantalla(){
	int angulo = 0;
	int x = 0;
	int y = 0;
	if (!this->controlador->comprobarCierreVentana()){
		for (this->opacidad = 0; this->opacidad <255; this->opacidad = this->opacidad+5){
			ventana->limpiar();
			texturaMenuFondo->aplicarPosicion(0,0,0,SDL_FLIP_NONE);
			texturaMenuMetalSlug->aplicarPosicion(180,40,0,SDL_FLIP_NONE);
			textoIngresePuerto->aplicarPosicion(312,337,0,SDL_FLIP_NONE);
			textoIngreseIP->aplicarPosicion(312,402,0,SDL_FLIP_NONE);
			campoTextoUno->aplicarPosicionDeFrame(310-x,358-y,0,-angulo,SDL_FLIP_NONE);
			campoTextoDos->aplicarPosicionDeFrame(310+x,423-y,0,angulo,SDL_FLIP_NONE);
			entradaTextoPuerto->aplicarPosicion(380-x,368-y,-angulo,SDL_FLIP_NONE);
			entradaTextoIP->aplicarPosicion(360+x,433-y,angulo,SDL_FLIP_NONE);
			texturaBotonSalir->aplicarPosicionDeFrame(656,550,0,0,SDL_FLIP_NONE);
			texturaBotonCreditos->aplicarPosicionDeFrame(30,550,0,0,SDL_FLIP_NONE);
			texturaBotonIngresar->aplicarPosicionDeFrame(338,475,0,0,SDL_FLIP_NONE);
			textoTPTaller->aplicarPosicion(254,560,0,SDL_FLIP_NONE);
			textoTPTaller->setAlpha(200);
			texturaEfectoLuz->aplicarPosicionConTamanio(0,0,800,600);
			texturaEfectoLuz->setAlpha(this->opacidad);
			angulo = angulo + 5;
			x = x+5;
			y = y+2;
			ventana->actualizar();
		}
	}
	this->opacidad = 230;
}

bool Vista::ventanaCerrada(){
	return controlador->comprobarCierreVentana();
}

void Vista::cerrar(){
	this->ventana->cerrar();
}
