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
	textura.push_back(ventana->crearBoton("Recursos/Boton_Conectar.png"));				//15: texturaBotonConectar
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

	for (this->opacidad; this->opacidad>0; this->opacidad=this->opacidad-3){
		ventana->limpiar();
		textura[0]->aplicarPosicion(0,0,0,SDL_FLIP_NONE);
		textura[1]->aplicarPosicion(180,40,0,SDL_FLIP_NONE);
		textura[2]->aplicarPosicion(200,430,0,SDL_FLIP_NONE);
		textura[11]->aplicarPosicion(254,560,0,SDL_FLIP_NONE);
		textura[11]->setAlpha(200);
		textura[3]->aplicarPosicionConTamanio(0,0,800,600);
		textura[3]->setAlpha(this->opacidad);
		ventana->actualizar();
	}

	bool incrementa = true;
	this->opacidad = 255;
	//Mientras la ventana no se cierre pulsando X o no se presione el enter, hacer el loop
	while ((!controlador->comprobarCierreVentana())&&(!controlador->presionarBoton(SDLK_RETURN))){
		ventana->limpiar();
		textura[0]->aplicarPosicion(0,0,0,SDL_FLIP_NONE);
		textura[1]->aplicarPosicion(180,40,0,SDL_FLIP_NONE);
		textura[2]->aplicarPosicion(200,430,0,SDL_FLIP_NONE);
		if (this->opacidad>=255)	incrementa = false;
		if (this->opacidad<=0) incrementa = true;
		if (incrementa){
			this->opacidad=this->opacidad+3;
		} else {
			this->opacidad=this->opacidad-3;
		}
		textura[2]->setAlpha(this->opacidad);
		textura[11]->aplicarPosicion(254,560,0,SDL_FLIP_NONE);
		textura[11]->setAlpha(200);
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
			textura[9]->actualizarTexto("Ingrese el puerto:",colorTexto);
			textura[10]->actualizarTexto("Ingrese la IP del servidor:",colorTexto);
			textura[14]->actualizarTexto("La dirección de ip o el puerto no permiten esta conexión",colorTextoAmarillo);
			break;
		case 3:
			campoUno = this->datos.nombre;
			campoDos = this->datos.contrasenia;
			textura[9]->actualizarTexto("Ingrese el nombre:",colorTexto);
			textura[10]->actualizarTexto("Ingrese la contraseña:",colorTexto);
			textura[14]->actualizarTexto("Usuario/contraseña incorrectos, inténtelo de nuevo",colorTextoAmarillo);
			break;
	}

	textura[12]->actualizarTexto(campoUno,colorTextoEntrada);
	textura[13]->actualizarTexto(campoDos,colorTextoEntrada);

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
			aviso = false;
		}

		if (campoUnoSeleccionado){
			this->controlador->ingresarCaracteresATexto(&campoUno);
			textura[7]->aplicarPosicionDeFrame(310,358,2,0,SDL_FLIP_NONE);
			textura[12]->actualizarTexto(campoUno,colorTextoEntrada);
		}

		if(textura[8]->aplicarPosicionDeBoton(310,423,&e)){
			campoUnoSeleccionado = false;
			campoDosSeleccionado = true;
			aviso = false;
		}

		if (campoDosSeleccionado){
			this->controlador->ingresarCaracteresATexto(&campoDos);
			textura[8]->aplicarPosicionDeFrame(310,423,2,0,SDL_FLIP_NONE);
			textura[13]->actualizarTexto(campoDos,colorTextoEntrada);
		}

		textura[12]->aplicarPosicion(316,363,0,SDL_FLIP_NONE);
		textura[13]->aplicarPosicion(316,428,0,SDL_FLIP_NONE);

		if (textura[4]->aplicarPosicionDeBoton(656,550,&e)){
			this->controlador->setCerrarVentana();
		}

		textura[5]->aplicarPosicionDeBoton(30,550,&e);

		switch(numeroPantalla){
			case 2:
				if(textura[6]->aplicarPosicionDeBoton(338,475,&e)){
					campoUnoSeleccionado = false;
					campoDosSeleccionado = false;
					this->datos.puerto = campoUno;
					this->datos.ip = campoDos;
					siguiente = true;
				}
				break;
			case 3:
				if(textura[15]->aplicarPosicionDeBoton(338,475,&e)){
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
					textura[14]->aplicarPosicion(205,530,0,SDL_FLIP_NONE);
					break;
				case 3:
					textura[14]->aplicarPosicion(215,530,0,SDL_FLIP_NONE);
					break;
			}
		}

		textura[11]->aplicarPosicion(254,560,0,SDL_FLIP_NONE);
		textura[11]->setAlpha(200);
		textura[3]->aplicarPosicionConTamanio(0,0,800,600);
		textura[3]->setAlpha(this->opacidad);
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
			textura[3]->setAlpha(this->opacidad);
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

