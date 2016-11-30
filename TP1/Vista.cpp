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
#include <sstream>
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
	this->pantallaPuntajes = false;
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
	textura.push_back(ventana->crearTexto("Recursos/font_puntajes.ttf",22));
	textura.push_back(ventana->crearTexto("Recursos/font_puntajes.ttf",22));
	textura.push_back(ventana->crearTexto("Recursos/font_puntajes.ttf",22));
	textura.push_back(ventana->crearTexto("Recursos/font_puntajes.ttf",22));
	textura.push_back(ventana->crearTextura("Recursos/Puntajes_fondo.png",0));
	textura.push_back(ventana->crearTexto("Recursos/msserif_bold.ttf",25));
	textura.push_back(ventana->crearTexto("Recursos/msserif_bold.ttf",25));
	textura.push_back(ventana->crearTexto("Recursos/msserif_bold.ttf",25));
	textura.push_back(ventana->crearTexto("Recursos/msserif_bold.ttf",25));
	textura.push_back(ventana->crearTexto("Recursos/font_puntajes.ttf",22));
	textura.push_back(ventana->crearTexto("Recursos/font_puntajes.ttf",22));
	textura.push_back(ventana->crearTexto("Recursos/font_puntajes.ttf",22));
	textura.push_back(ventana->crearTexto("Recursos/font_puntajes.ttf",22));

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
	#define texturaPuntajesFondo textura[23]
	#define textoPuntajesNivel textura[24]
	#define textoMisionCompleta textura[25]
	#define textoPuntajesGlobal textura[26]
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
	TexturaSDL* imgEnergia = ventana->crearTextura("Recursos/energia.png",0);
	TexturaSDL* energia = ventana->crearTexto("Recursos/msserif_bold.ttf",16);
	TexturaSDL* imgPuntaje = ventana->crearTextura("Recursos/puntaje.png",0);
	TexturaSDL* puntaje = ventana->crearTexto("Recursos/msserif_bold.ttf",16);
	TexturaSDL* texturaNombre = ventana->crearTexto("Recursos/msserif_bold.ttf", 18);
	vistaJugador = new VistaJugador(nombre,x,y,(ventana->crearTextura("Recursos/" + sprite->getPath() + ".png", atoi(fotogramas.c_str()))), SDL_FLIP_NONE,energia,imgEnergia,puntaje,imgPuntaje, texturaNombre);
	vistaJugador->texturaNombre->actualizarTexto(vistaJugador->nombre,{255,255,255});
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

void Vista::inicializarCamara(int camaraX, int camaraY, int anchoVentana, int altoVentana, vector<pair<int,int>> abscisasCapas, vector<ImagenDto*> imagenes, vector<string> nombreCapas)
{
	camara = {camaraX,camaraY,anchoVentana,altoVentana};
	for (int i=0; i<abscisasCapas.size(); i++)
	{
		SDL_Rect rectangulo = {abscisasCapas.at(i).first,0,anchoVentana,altoVentana};
		for (int j = 0; j < imagenes.size(); j++)
		{
			if (imagenes.at(j)->getPath() == nombreCapas.at(i))
			{
				Capa* capa = new Capa(imagenes.at(j), rectangulo, ventana->crearTextura("Recursos/" + imagenes.at(j)->getPath() + ".png",0));
				vectorCapas.push_back(capa);
			}
		}
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

void Vista::actualizarPuntaje(string jugador, int puntaje){
	for (int i = 0; i < vistaJugadores.size(); i++){
		VistaJugador* vistaJugador = vistaJugadores.at(i);
		if (vistaJugador->nombre == jugador)
		{
			vistaJugador->valorPuntaje = puntaje;
			break;
		}
	}
}

void Vista::actualizarVida(string jugador, int vida){
	for (int i = 0; i < vistaJugadores.size(); i++){
		VistaJugador* vistaJugador = vistaJugadores.at(i);
		if (vistaJugador->nombre == jugador)
		{
			vistaJugador->valorEnergia = vida;
			if (vida == 0)
			{
				vistaJugadores.erase(vistaJugadores.begin()+i);
			}
			break;
		}
	}
}

void Vista::actualizarPantalla(int anchoVentana, int anchoCapaPrincipal) {
	this->ventana->limpiar();

	for (int i=vectorCapas.size()-1; i>=0; i--)
	{
		vectorCapas.at(i)->paralajeInfinito(anchoVentana, i);
	}
	TexturaSDL* texturaJugadorX;
	TexturaSDL* texturaBala;
	TexturaSDL* texturaEnemigo;
	TexturaSDL* texturaItem;
	TexturaSDL* texturaBoss;

	for (int i = 0; i < vistaItems.size(); i++) {
		Item* vistaItem = vistaItems.at(i);
		texturaItem = vistaItem->textura;
		texturaItem->aplicarPosicion(vistaItem->boxCollider.x - camara.x, vistaItem->boxCollider.y - camara.y,0,SDL_FLIP_NONE);
	}

	for (int i = 0; i < vistaBalas.size(); i++) {
		VistaBala* vistaBala = vistaBalas.at(i);
		texturaBala = vistaBala->textura;
		texturaBala->aplicarPosicion(vistaBala->x - camara.x, vistaBala->y - camara.y,vistaBala->angulo,vistaBala->flip);
	}

	for (int i = 0; i < vistaEnemigos.size(); i++) {
		VistaEnemigo* vistaEnemigo = vistaEnemigos.at(i);
		texturaEnemigo = vistaEnemigo->textura;
		texturaEnemigo->aplicarPosicion(vistaEnemigo->x - camara.x, vistaEnemigo->y - camara.y,0,SDL_FLIP_NONE);
	}
	if (this->vistaBoss != NULL)
	{
		texturaBoss = this->vistaBoss->textura;
		texturaBoss->aplicarPosicion(vistaBoss->x - camara.x, vistaBoss->y - camara.y,0,vistaBoss->flip);
	}

	for (int i = 0; i < vistaJugadores.size(); i++) {
		VistaJugador* vistaJugador = vistaJugadores.at(i);
		texturaJugadorX = vistaJugador->texturaJugador;
		if (vistaJugador->x > anchoCapaPrincipal && camara.x == 0) {
			vistaJugador->x = vistaJugador->x - anchoCapaPrincipal;
		}
		texturaJugadorX->aplicarPosicion(vistaJugador->x - camara.x,vistaJugador->y - camara.y,0,vistaJugador->flip);
		vistaJugador->imagenEnergia->aplicarPosicion(vistaJugador->x - camara.x + texturaJugadorX->getAnchoSprite()/2 - 40, vistaJugador->y + 80, 0, SDL_FLIP_NONE); //44 es la mitad del boxCollider
		vistaJugador->energia->aplicarPosicion(vistaJugador->x - camara.x + texturaJugadorX->getAnchoSprite()/2 - 20, vistaJugador->y + 80, 0, SDL_FLIP_NONE);
		vistaJugador->energia->actualizarTexto(to_string(vistaJugador->valorEnergia),{255,255,255});
		vistaJugador->imagenPuntaje->aplicarPosicion(vistaJugador->x - camara.x + texturaJugadorX->getAnchoSprite()/2 + 15, vistaJugador->y + 80, 0, SDL_FLIP_NONE);
		vistaJugador->puntaje->aplicarPosicion(vistaJugador->x - camara.x + texturaJugadorX->getAnchoSprite()/2 + 35, vistaJugador->y + 80, 0, SDL_FLIP_NONE);
		vistaJugador->puntaje->actualizarTexto(to_string(vistaJugador->valorPuntaje),{255,255,255});
		vistaJugador->texturaNombre->aplicarPosicion(vistaJugador->x - camara.x + texturaJugadorX->getAnchoSprite()/2 - vistaJugador->texturaNombre->getAncho()/2,vistaJugador->y + 60, 0, SDL_FLIP_NONE);
	}

	if (pantallaPuntajes){
		texturaPuntajesFondo->aplicarPosicion(0,0,0,SDL_FLIP_NONE);

		textoMisionCompleta->actualizarTexto("MISIÓN COMPLETADA", {255,255,255});
		textoMisionCompleta->aplicarPosicion(ANCHO_VENTANA/2 - textoMisionCompleta->getAncho()/2 ,40,0,SDL_FLIP_NONE);

		textoPuntajesNivel->actualizarTexto("SCOREBOARD NIVEL", {255,255,255});
		textoPuntajesNivel->aplicarPosicion(ANCHO_VENTANA/2 - textoPuntajesNivel->getAncho()/2 ,80,0,SDL_FLIP_NONE);

		textoPuntajesGlobal->actualizarTexto("SCOREBOARD GLOBAL", {255,255,255});
		textoPuntajesGlobal->aplicarPosicion(ANCHO_VENTANA/2 - textoPuntajesGlobal->getAncho()/2 ,280,0,SDL_FLIP_NONE);

		for (int i = 0; i < vistaJugadores.size(); i++){
			VistaJugador* jugador = vistaJugadores.at(i);
			string nombre = jugador->nombre;
			if (modoJuego == 3){
				nombre += "(Equipo " + jugador->equipo + ")";
			}
			textura[i + 19]->actualizarTexto(nombre + "                " + to_string(jugador->valorPuntaje), {255,255,255});
			textura[i + 19]->aplicarPosicion(ANCHO_VENTANA/2 - textura[i+19]->getAncho()/2, 120 + i * 40,0,SDL_FLIP_NONE);
			textura[i + 28]->actualizarTexto(nombre + "                " + to_string(jugador->puntajeTotal), {255,255,255});
			textura[i + 28]->aplicarPosicion(ANCHO_VENTANA/2 - textura[i+28]->getAncho()/2, 320 + i * 40,0,SDL_FLIP_NONE);
		}

		TexturaSDL* textoNivel = ventana->crearTexto("Recursos/msserif_bold.ttf",25);
		TexturaSDL* textoTotal = ventana->crearTexto("Recursos/msserif_bold.ttf",25);

		if (this->modoJuego == 2){
			textoNivel->actualizarTexto("Puntaje Acumulado Nivel: " + to_string(puntajeSumadoNivel), {255,255,255});
			textoNivel->aplicarPosicion(ANCHO_VENTANA/2 - textoNivel->getAncho()/2, 480,0,SDL_FLIP_NONE);
			textoTotal->actualizarTexto("Puntaje Acumulado Total: " + to_string(puntajeSumadoTotal), {255,255,255});
			textoTotal->aplicarPosicion(ANCHO_VENTANA/2 - textoTotal->getAncho()/2, 520,0,SDL_FLIP_NONE);
		}
		else if (this->modoJuego == 3){
			TexturaSDL* textoNivel = ventana->crearTexto("Recursos/msserif_bold.ttf",25);
			TexturaSDL* textoTotal = ventana->crearTexto("Recursos/msserif_bold.ttf",25);
			textoNivel->actualizarTexto("Puntaje Equipo Alfa Nivel: " + to_string(puntajeEquipoAlfa)+ ". Acumulado: " + to_string(puntajeTotalEquipoAlfa), {255,255,255});
			textoNivel->aplicarPosicion(ANCHO_VENTANA/2 - textoNivel->getAncho()/2, 480,0,SDL_FLIP_NONE);
			textoTotal->actualizarTexto("Puntaje Equipo Beta Nivel: " + to_string(puntajeEquipoBeta) + ". Acumulado: " + to_string(puntajeTotalEquipoBeta), {255,255,255});
			textoTotal->aplicarPosicion(ANCHO_VENTANA/2 - textoTotal->getAncho()/2, 520,0,SDL_FLIP_NONE);
		}
	}

	if (texturaBotonDesconectar->aplicarPosicionDeBoton(10,10,&evento)) {
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
						vistaBala->~VistaBala();
						break;
					}
				}
			}
		}
	}
}

void Vista::actualizarEnemigo(string enemigo, int x, int y, string sprite, int id, int cantFotogramas) {
	//si viene un nuevo enemigo
	if (enemigo == "0") {
		VistaEnemigo* vistaEnemigo = new VistaEnemigo(x,y,(ventana->crearTextura("Recursos/" + sprite + ".png", cantFotogramas)),id, "Normal");
		vistaEnemigos.push_back(vistaEnemigo);
	} else {
		//si viene un enemigo ya existente
		if (enemigo == "1") {
			for (int i = 0; i < vistaEnemigos.size(); i++){
				VistaEnemigo* vistaEnemigo = vistaEnemigos.at(i);
				if (vistaEnemigo->id == id) {
					vistaEnemigo->x = x;
					vistaEnemigo->y = y;
				}
			}
		} else {
			//si ese enemigo se murio
			if (enemigo == "2") {
				for (int i = 0; i < vistaEnemigos.size(); i++){
					VistaEnemigo* vistaEnemigo = vistaEnemigos.at(i);
					if (vistaEnemigo->id == id) {
						vistaEnemigos.erase(vistaEnemigos.begin() + i);
						break;
					}
				}
			}
		}
	}
}

void Vista::agregarVistaItem(string borrarItem, string sprite, int x, int y, int cantFotogramas)
{
	if (borrarItem == "0")
	{
		Item* vistaItem = new Item(x,y,(ventana->crearTextura("Recursos/" + sprite + ".png", cantFotogramas)));
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

void Vista::actualizarPuntajesFinNivel(int modoJuego, vector<string> jugadoresPuntajes){
	this->modoJuego = modoJuego;
	puntajeSumadoNivel = 0;
	puntajeSumadoTotal = 0;
	puntajeEquipoAlfa = 0;
	puntajeEquipoBeta = 0;
	puntajeTotalEquipoAlfa = 0;
	puntajeTotalEquipoBeta = 0;
	for (int i = 0; i < jugadoresPuntajes.size(); i++){
		vector<string> puntajeJugador = splitToVec(jugadoresPuntajes.at(i), ',');
		string nombreJugador = puntajeJugador.at(0);
		for (int i = 0; i < vistaJugadores.size(); i++){
			int puntajeNivel = stringToInt(puntajeJugador.at(1));
			int puntajeTotal = stringToInt(puntajeJugador.at(2));
			VistaJugador* jugador = vistaJugadores.at(i);
			if (jugador->nombre == nombreJugador){
				jugador->valorPuntaje = puntajeNivel;
				jugador->puntajeTotal = puntajeTotal;
				if (modoJuego == 3){
					jugador->equipo = puntajeJugador.at(3);
					if (jugador->equipo == "alfa"){
						puntajeEquipoAlfa += jugador->valorPuntaje;
						puntajeTotalEquipoAlfa += jugador->puntajeTotal;
					}
					else{
						puntajeEquipoBeta += jugador->valorPuntaje;
						puntajeTotalEquipoBeta += jugador->puntajeTotal;
					}
				}
				if (modoJuego == 2){
					puntajeSumadoNivel += jugador->valorPuntaje;
					puntajeSumadoTotal += jugador->puntajeTotal;
				}
			}
		}
	}
}

void Vista::actualizarBoss(string boss, int x, int y, string sprite, string sentido, int cantFotogramas)
{
	if (boss == "0")
	{
		VistaEnemigo* vistaBoss = new VistaEnemigo(x,y,(ventana->crearTextura("Recursos/" + sprite + ".png", cantFotogramas)),0, sentido);
		this->vistaBoss = vistaBoss;
	}
	else if (boss == "1")
	{
		vistaBoss->x = x;
		vistaBoss->y = y;
		vistaBoss->verificarSentido(sentido);
	}
	else if (boss == "2")
	{
		vistaBoss->~VistaEnemigo();
	}
}

void Vista::split(const std::string &s, char delim, std::vector<std::string> &elems) {
	//splitea un string segun un delim y lo guarda en elems
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
}


std::vector<std::string> Vista::splitToVec(const std::string &s, char delim) {
	//devuelve un vector de elementos que representan al string spliteado segun delim
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}


int Vista::stringToInt(string atributo) {
	istringstream atributoStream(atributo);
	int atributoInt;
	atributoStream >> atributoInt;

	return atributoInt;
}
