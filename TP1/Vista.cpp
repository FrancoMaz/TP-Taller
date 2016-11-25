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
#include <unistd.h>
#include <iostream>

#define PI 3.14159265

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
	textura.push_back(ventana->crearTextura("Recursos/Fondo_Escenario_capa_1.png",0));
	textura.push_back(ventana->crearBoton("Recursos/Boton_Salir.png"));
	textura.push_back(ventana->crearTexto("Recursos/msserif_bold.ttf",25));

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
	#define texturaFondoEscenario textura[16]
	#define texturaBotonDesconectar textura[17]
	#define textoEsperandoJugadores textura[18]
}

void Vista::cargarPrimeraPantalla(){
	for (float y=-290; y<40; y=y+5){
		this->ventana->limpiar();
		texturaMenuFondo->aplicarPosicion(0,0,0,SDL_FLIP_NONE);
		texturaMenuMetalSlug->aplicarPosicion(180,y,0,SDL_FLIP_NONE);
		this->ventana->actualizar();
	}

	SDL_Color colorTexto = {255,255,255};
	textoTPTaller->actualizarTexto("Trabajo Práctico - Taller de Programación", colorTexto);

	for (this->opacidad; this->opacidad>0; this->opacidad=this->opacidad-3){
		this->ventana->limpiar();
		texturaMenuFondo->aplicarPosicion(0,0,0,SDL_FLIP_NONE);
		texturaMenuMetalSlug->aplicarPosicion(180,40,0,SDL_FLIP_NONE);
		texturaPresionarEnter->aplicarPosicion(200,430,0,SDL_FLIP_NONE);
		textoTPTaller->aplicarPosicion(254,560,0,SDL_FLIP_NONE);
		textoTPTaller->setAlpha(200);
		texturaEfectoLuz->aplicarPosicionConTamanio(0,0,800,600);
		texturaEfectoLuz->setAlpha(this->opacidad);
		this->ventana->actualizar();
	}

	bool incrementa = true;
	bool botonEnter = false;
	this->opacidad = 255;
	//Mientras la ventana no se cierre pulsando X o no se presione el enter, hacer el loop
	while ((!controlador->comprobarCierreVentana())&&(!botonEnter)){
		this->ventana->limpiar();
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
		this->ventana->actualizar();

		while(SDL_PollEvent(&evento) != 0){
			botonEnter = controlador->presionarBoton(SDLK_RETURN);
		}
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
	string campoUno;
	string campoDos;
	bool campoUnoSeleccionado = false;
	bool campoDosSeleccionado = false;
	bool siguiente = false;

	//Inicializamos la entrada para textos
	SDL_StartTextInput();

	switch(numeroPantalla){
		case 2:
			//campoUno = this->datos.puerto;
			campoUno = "7891";
			//campoDos = this->datos.ip;
			campoDos = "127.0.0.1";
			//campoDos = "192.168.1.12";
			textoIngresePuerto->actualizarTexto("Ingrese el puerto:",colorTexto);
			textoIngreseIP->actualizarTexto("Ingrese la IP del servidor:",colorTexto);
			textoDatosNoCoinciden->actualizarTexto("La dirección de ip o el puerto no permiten esta conexión",colorTextoAmarillo);
			break;
		case 3:
			campoUno = this->datos.nombre;
			campoDos = "123456";
			//campoDos = this->datos.contrasenia;
			textoIngresePuerto->actualizarTexto("Ingrese el nombre:",colorTexto);
			textoIngreseIP->actualizarTexto("Ingrese la contraseña:",colorTexto);
			textoDatosNoCoinciden->actualizarTexto("Usuario/contraseña incorrectos, inténtelo de nuevo",colorTextoAmarillo);
			break;
	}

	entradaTextoPuerto->actualizarTexto(campoUno,colorTextoEntrada);
	entradaTextoIP->actualizarTexto(campoDos,colorTextoEntrada);

	while ((!this->controlador->comprobarCierreVentana())&&(!siguiente)){
		this->ventana->limpiar();
		texturaMenuFondo->aplicarPosicion(0,0,0,SDL_FLIP_NONE);
		texturaMenuMetalSlug->aplicarPosicion(180,40,0,SDL_FLIP_NONE);

		textoIngresePuerto->aplicarPosicion(312,337,0,SDL_FLIP_NONE);
		textoIngreseIP->aplicarPosicion(312,402,0,SDL_FLIP_NONE);

		if(campoTextoUno->aplicarPosicionDeBoton(310,358,&evento)){
			campoUnoSeleccionado = true;
			campoDosSeleccionado = false;
			aviso = false;
		}

		if (campoUnoSeleccionado){
			this->controlador->ingresarCaracteresATexto(&campoUno);
			campoTextoUno->aplicarPosicionDeFrame(310,358,2,0,SDL_FLIP_NONE);
			entradaTextoPuerto->actualizarTexto(campoUno,colorTextoEntrada);
		}

		if(campoTextoDos->aplicarPosicionDeBoton(310,423,&evento)){
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

		if (texturaBotonSalir->aplicarPosicionDeBoton(656,550,&evento)){
			this->controlador->setCerrarVentana();
		}

		texturaBotonCreditos->aplicarPosicionDeBoton(30,550,&evento);

		switch(numeroPantalla){
			case 2:
				if(texturaBotonIngresar->aplicarPosicionDeBoton(338,475,&evento)){
					campoUnoSeleccionado = false;
					campoDosSeleccionado = false;
					this->datos.puerto = campoUno;
					this->datos.ip = campoDos;
					siguiente = true;
				}
				break;
			case 3:
				if(texturaBotonConectar->aplicarPosicionDeBoton(338,475,&evento)){
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
		this->ventana->actualizar();

		this->controlador->botonCerrarVentanaSeleccionado();

		this->opacidad = this->opacidad-5;
		if (this->opacidad <= 0) this->opacidad = 0;
	}
	SDL_StopTextInput();
	return this->datos;
}

void Vista::cargarPantallaEsperandoJugadores(){
	this->ventana->limpiar();

	texturaMenuFondo->aplicarPosicion(0,0,0,SDL_FLIP_NONE);
	texturaMenuMetalSlug->aplicarPosicion(180,40,0,SDL_FLIP_NONE);

	textoEsperandoJugadores->actualizarTexto("ESPERANDO JUGADORES...",{255,255,255});
	textoEsperandoJugadores->setAlpha(220);
	textoEsperandoJugadores->aplicarPosicion(212,410,0,SDL_FLIP_NONE);

	textoTPTaller->aplicarPosicion(254,560,0,SDL_FLIP_NONE);
	textoTPTaller->setAlpha(200);

	this->ventana->actualizar();
	this->controlador->botonCerrarVentanaSeleccionado();
}

void Vista::transicionDePantalla(){
	int angulo = 0;
	int x = 0;
	int y = 0;
	if (!this->controlador->comprobarCierreVentana()){
		for (this->opacidad = 0; this->opacidad <255; this->opacidad = this->opacidad+5){
			this->ventana->limpiar();
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
			this->ventana->actualizar();
		}
	}
	this->opacidad = 230;
}

void Vista::cargarEscenario(vector<ImagenDto*> imagenes, int anchoVentana, int altoVentana){
	this->ventana->limpiar();
	for (int i=vectorCapas.size()-1; i>=0; i--)
	{
		vectorCapas.at(i)->textura->aplicarPosicionDePorcion(0,0,&vectorCapas.at(i)->rectangulo, 0, SDL_FLIP_NONE);
	}
	for (int i = 0; i < vistaJugadores.size(); i++){
		VistaJugador* vistaJugador = vistaJugadores.at(i);
		vistaJugador->texturaJugador->aplicarPosicion(vistaJugador->x,vistaJugador->y,0,SDL_FLIP_NONE);
	}
	//this->ventana->actualizar();
}

void Vista::actualizarJugador(UpdateJugador* update, int anchoVentana, int anchoCapaPrincipal)
{
	this->ventana->limpiar();
	for (int i=vectorCapas.size()-1; i>=0; i--)
	{
		vectorCapas.at(i)->paralajeInfinito(anchoVentana, i);
	}
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	TexturaSDL* texturaJugadorX;
	for (int i = 0; i < vistaJugadores.size(); i++){
		VistaJugador* vistaJugador = vistaJugadores.at(i);
		texturaJugadorX = vistaJugador->texturaJugador;
		if (vistaJugador->nombre == update->getRemitente())
		{
			vistaJugador->x = atoi(update->getX().c_str());
			vistaJugador->y = atoi(update->getY().c_str());
			texturaJugadorX->cargarImagen("Recursos/" + update->getSpriteActual()->getPath() + ".png");
			texturaJugadorX->generarSprite(atoi(update->getSpriteActual()->getCantidadDeFotogramas().c_str()));
			if (update->getCondicion() == "Normal")
			{
				vistaJugador->flip = SDL_FLIP_NONE;
			}
			else
			{
				vistaJugador->flip = SDL_FLIP_HORIZONTAL;
			}
		}
		if (vistaJugador->x > anchoCapaPrincipal && camara.x == 0)
		{
			vistaJugador->x = vistaJugador->x - anchoCapaPrincipal;
		}
		texturaJugadorX->aplicarPosicion(vistaJugador->x - camara.x,vistaJugador->y - camara.y,0,vistaJugador->flip);
	}
	if (texturaBotonDesconectar->aplicarPosicionDeBoton(10,10,&evento))
	{
		controlador->setCerrarVentana();
	}
	this->ventana->actualizar();
}

bool Vista::ventanaCerrada(){
	return controlador->comprobarCierreVentana();
}

void Vista::cerrar(){
	this->ventana->cerrar();
}

void Vista::cargarVistaInicialJugador(string nombre, int x, int y, SpriteDto* sprite)
{
	string fotogramas = sprite->getCantidadDeFotogramas();
	VistaJugador* vistaJugador;
	vistaJugador = new VistaJugador(nombre,x,y,(ventana->crearTextura("Recursos/" + sprite->getPath() + ".png", atoi(fotogramas.c_str()))), SDL_FLIP_NONE);
	vistaJugadores.push_back(vistaJugador);
}

void Vista::actualizarCamara(int x, int y, vector<pair<int,int>> abscisasCapas, int anchoVentana)
{
	for (int i=vectorCapas.size()-1; i>=0; i--)
	{
		vectorCapas.at(i)->rectangulo.x = abscisasCapas.at(i).first;
		vectorCapas.at(i)->vel = abscisasCapas.at(i).second;
		vectorCapas.at(i)->paralajeInfinito(anchoVentana, i);
	}
	camara.x = vectorCapas.at(0)->rectangulo.x;
	camara.y = vectorCapas.at(0)->rectangulo.y;
}

void Vista::inicializarCamara(int camaraX, int camaraY, int anchoVentana, int altoVentana, vector<pair<int,int>> abscisasCapas, vector<ImagenDto*> imagenes)
{
	camara = {camaraX,camaraY,anchoVentana,altoVentana};
	for (int i=0; i<abscisasCapas.size(); i++)
	{
		SDL_Rect rectangulo = {abscisasCapas.at(i).first,0,anchoVentana,altoVentana};
		Capa* capa = new Capa(imagenes.at(i), rectangulo, ventana->crearTextura("Recursos/" + imagenes.at(i)->getPath() + ".png",0));
		vectorCapas.push_back(capa);
	}
}

void Vista::resetearVistas(int anchoCapaPrincipal)
{
	for (int i = 0; i < vistaJugadores.size(); i++)
	{
		VistaJugador* vistaJugador = vistaJugadores.at(i);
		vistaJugador->x = vistaJugador->x - anchoCapaPrincipal;
		vistaJugador->texturaJugador->aplicarPosicion(vistaJugador->x - camara.x,vistaJugador->y - camara.y,0,vistaJugador->flip);
	}
}

void Vista::actualizarPosJugador(UpdateJugador* update, int anchoVentana, int anchoCapaPrincipal)
{
	TexturaSDL* texturaJugadorX;
	for (int i = 0; i < vistaJugadores.size(); i++){
		VistaJugador* vistaJugador = vistaJugadores.at(i);
		texturaJugadorX = vistaJugador->texturaJugador;
		if (vistaJugador->nombre == update->getRemitente())
		{
			vistaJugador->x = atoi(update->getX().c_str());
			vistaJugador->y = atoi(update->getY().c_str());
			if(update->getSpriteAnterior() != update->getSpriteActual()){
				update->setSpriteAnterior(update->getSpriteActual());
				texturaJugadorX->cargarImagen("Recursos/" + update->getSpriteActual()->getPath() + ".png");
				texturaJugadorX->generarSprite(atoi(update->getSpriteActual()->getCantidadDeFotogramas().c_str()));
			}
			if (update->getCondicion() == "Normal")
			{
				vistaJugador->flip = SDL_FLIP_NONE;
			}
			else
			{
				vistaJugador->flip = SDL_FLIP_HORIZONTAL;
			}
			if (vistaJugador->nombre == update->getDestinatario()){
				if (vistaJugador->y == PISO || vistaJugador->y == PLATAFORMA){
					salto = false;
				}
				else{
					salto = true;
				}
			}
			if (vistaJugador->x > anchoCapaPrincipal && camara.x == 0)
			{
				vistaJugador->x = vistaJugador->x - anchoCapaPrincipal;
			}
			break;
		}
	}
}

void Vista::actualizarPantalla(int anchoVentana, int anchoCapaPrincipal){
	this->ventana->limpiar();
	for (int i=vectorCapas.size()-1; i>=0; i--)
	{
		vectorCapas.at(i)->paralajeInfinito(anchoVentana, i);
	}
	TexturaSDL* texturaJugadorX;
	TexturaSDL* texturaBala;
	TexturaSDL* texturaItem;

	for (int i = 0; i < vistaItems.size(); i++){
			Item* vistaItem = vistaItems.at(i);
			texturaItem = vistaItem->textura;
			texturaItem->aplicarPosicion(vistaItem->boxCollider.x - camara.x, vistaItem->boxCollider.y - camara.y,0,SDL_FLIP_NONE);
		}

	for (int i = 0; i < vistaBalas.size(); i++){
		VistaBala* vistaBala = vistaBalas.at(i);
		texturaBala = vistaBala->textura;
		texturaBala->aplicarPosicion(vistaBala->x - camara.x, vistaBala->y - camara.y,vistaBala->angulo,vistaBala->flip);
	}

	for (int i = 0; i < vistaJugadores.size(); i++){
		VistaJugador* vistaJugador = vistaJugadores.at(i);
		texturaJugadorX = vistaJugador->texturaJugador;
		if (vistaJugador->x > anchoCapaPrincipal && camara.x == 0)
		{
			vistaJugador->x = vistaJugador->x - anchoCapaPrincipal;
		}
		texturaJugadorX->aplicarPosicion(vistaJugador->x - camara.x,vistaJugador->y - camara.y,0,vistaJugador->flip);
	}

	if (texturaBotonDesconectar->aplicarPosicionDeBoton(10,10,&evento))
	{
		controlador->setCerrarVentana();
	}
	this->ventana->actualizar();
}

void Vista::actualizarProyectil(string nuevaBala, int x, int y, string sprite, int id, int cantFotogramas, string sentido, double angulo) {
	if (nuevaBala == "0") {
		VistaBala* vistaBala = new VistaBala(x,y,(ventana->crearTextura("Recursos/" + sprite + ".png", cantFotogramas)),id,sentido,angulo);
		vistaBalas.push_back(vistaBala);
	} else {
		if (nuevaBala == "1") {
			for (int i = 0; i < vistaBalas.size(); i++){
				VistaBala* vistaBala = vistaBalas.at(i);
				if (vistaBala->id == id) {
					vistaBala->x = x;
					vistaBala->y = y;
				}
			}
		} else {
			if (nuevaBala == "2") {
				for (int i = 0; i < vistaBalas.size(); i++){
					VistaBala* vistaBala = vistaBalas.at(i);
					if (vistaBala->id == id) {
						vistaBalas.erase(vistaBalas.begin() + i);
						break;
					}
				}
			}
		}
	}
}

void Vista::agregarVistaItem(string borrarItem, string sprite, int x, int y)
{
	if (borrarItem == "0")
	{
		Item* vistaItem = new Item(x,y,(ventana->crearTextura("Recursos/" + sprite + ".png", 0)));
		vistaItems.push_back(vistaItem);
	}
	else if (borrarItem == "1")
	{
		for (int i = 0; i < vistaItems.size(); i++)
		{
			Item* vistaItem = vistaItems.at(i);
			if (vistaItem->boxCollider.x == x)
			{
				vistaItems.erase(vistaItems.begin() + i);
				break;
			}
		}
	}
}
