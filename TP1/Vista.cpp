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
	textura.push_back(ventana->crearTextura("Recursos/Jugador.png",3));
	textura.push_back(ventana->crearTextura("Recursos/TP2_test/Fondo_Escenario_capa_1_test.png",0));
	textura.push_back(ventana->crearTextura("Recursos/TP2_test/Fondo_Escenario_capa_2_test.png",0));
	textura.push_back(ventana->crearTextura("Recursos/TP2_test/Fondo_Escenario_capa_3_test.png",0));
	textura.push_back(ventana->crearTextura("Recursos/TP2_test/Fondo_Escenario_capa_4_test.png",0));

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
	#define texturaJugador textura[16]
	#define texturaFondoEscenarioCapaUno textura[17]
	#define texturaFondoEscenarioCapaDos textura[18]
	#define texturaFondoEscenarioCapaTres textura[19]
	#define texturaFondoEscenarioCapaCuatro textura[20]
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

void Vista::cargarEscenario(){
	int jugador_X = 20;
	int jugador_Y = 410;
	int capa_X = 0;
	int anchoEscenario = texturaFondoEscenarioCapaUno->getAncho();
	int altoEscenario = texturaFondoEscenarioCapaUno->getAlto();
	int velocidad_X = 0;
	int velocidad_Y = 0;
	int velocidad_capaDos_X, velocidad_capaTres_X = 0;
	bool saltar = false;
	int const velocidad = 3;
	double angulo = 0;
	SDL_Rect camara = {0,0,ANCHO_VENTANA,ALTO_VENTANA};

	//Los siguientes rectángulos se encargan de renderizar cada capa
	SDL_Rect capaNubes = {0,0,ANCHO_VENTANA,ALTO_VENTANA};
	SDL_Rect capaDosArena = {0,0,ANCHO_VENTANA,ALTO_VENTANA};
	SDL_Rect capaTresArena = {0,0,ANCHO_VENTANA,ALTO_VENTANA};
	SDL_Rect capaCiudad = {0,0,ANCHO_VENTANA,ALTO_VENTANA};

	SDL_RendererFlip flip = SDL_FLIP_NONE;

	float velocidad_nubes = 0;

	while(!this->controlador->comprobarCierreVentana()){
		this->ventana->limpiar();
		while(SDL_PollEvent(&evento)){
			//Si presiono las teclas
			if(this->controlador->presionarBoton(SDLK_RIGHT)){
				velocidad_X += velocidad;
				if(!saltar){
					texturaJugador->cargarImagen("Recursos/Jugador_corriendo.png");
					texturaJugador->generarSprite(9);
				}
				flip = SDL_FLIP_NONE;
			}

			if(this->controlador->presionarBoton(SDLK_LEFT)){
				velocidad_X -= velocidad;
				if(!saltar){
					texturaJugador->cargarImagen("Recursos/Jugador_corriendo.png");
					texturaJugador->generarSprite(9);
				}
				flip = SDL_FLIP_HORIZONTAL;
			}

			if(this->controlador->presionarBoton(SDLK_UP)){
				if(angulo == 0){
					saltar = true;
					texturaJugador->cargarImagen("Recursos/Jugador_saltando.png");
					texturaJugador->generarSprite(9);
				}
			}

			if(this->controlador->presionarBoton(SDLK_DOWN)){

			}

			//Si suelto las teclas
			if(this->controlador->soltarBoton(SDLK_RIGHT)){
				velocidad_X -= velocidad;
				if(!saltar){
					texturaJugador->cargarImagen("Recursos/Jugador.png");
					texturaJugador->generarSprite(3);
				}
				flip = SDL_FLIP_NONE;
			}

			if(this->controlador->soltarBoton(SDLK_LEFT)){
				velocidad_X += velocidad;
				if(!saltar){
					texturaJugador->cargarImagen("Recursos/Jugador.png");
					texturaJugador->generarSprite(3);
				}
				flip = SDL_FLIP_HORIZONTAL;
			}

			if(this->controlador->soltarBoton(SDLK_UP)){

			}

			if(this->controlador->soltarBoton(SDLK_DOWN)){

			}
		}

		//Compruebo el salto
		if(saltar){
			velocidad_Y = -7*cos(angulo);
			angulo += PI/50;
			if (angulo > (PI + (PI/50))){
				angulo = 0;
				saltar = false;
				if (velocidad_X != 0){
					texturaJugador->cargarImagen("Recursos/Jugador_corriendo.png");
					texturaJugador->generarSprite(9);
				} else {
					texturaJugador->cargarImagen("Recursos/Jugador.png");
					texturaJugador->generarSprite(3);
				}
			}
		} else {
			velocidad_Y = 0;
		}

		//Actualizo las posiciones del objeto
		jugador_X += velocidad_X;
		jugador_Y += velocidad_Y;

		//Para que el jugador no se vaya de pantalla
		if(jugador_X < camara.x){
			//Muevo para atrás
			jugador_X -= velocidad_X;
		}

		if((jugador_Y < 0)||(jugador_Y + texturaJugador->getAltoSprite() > altoEscenario)){
			//Muevo para atrás
			jugador_Y -= velocidad_Y;
		}

		//Centro la cámara en el jugador
		//Con esta condición hago que la cámara no pueda volver hacia atras cuando el jugador avanzó por el escenario
		if((jugador_X + texturaJugador->getAnchoSprite()/2) - camara.x > ANCHO_VENTANA/2){
			camara.x = (jugador_X + texturaJugador->getAnchoSprite()/2) - ANCHO_VENTANA/2;

			//Establezco la velocidad de las capas del fondo
			velocidad_capaDos_X += velocidad_X;
			velocidad_capaTres_X += velocidad_X;
		}


		camara.y = (jugador_Y + texturaJugador->getAltoSprite()/2) - ALTO_VENTANA/2;

		//Mantengo la cámara dentro de los límites del escenario
		if (camara.x < 0){
			camara.x = 0;
		}

		if (camara.y < 0){
			camara.y = 0;
		}

		if (camara.y > altoEscenario - camara.h){
			camara.y = altoEscenario - camara.h;
		}

		//Establezco el movimiento de la capa de las nubes
		capaNubes.y = camara.y;
		velocidad_nubes = velocidad_nubes - 0.2;
		capaNubes.x = -velocidad_nubes;

		/* Paralaje infinito de nubes */
		//Si la textura comienza a salirse de pantalla
		if((capaNubes.x + ANCHO_VENTANA) > texturaFondoEscenarioCapaCuatro->getAncho()){
			//Creo una posición auxiliar para no perder el valor de capaNubes.x
			int posicionAuxiliar = 0;
			//Renderizo la porción derecha de la textura saliente y la posiciono en x=0
			capaNubes.w = texturaFondoEscenarioCapaCuatro->getAncho() - capaNubes.x;
			texturaFondoEscenarioCapaCuatro->aplicarPosicionDePorcion(0,0,&capaNubes,0,SDL_FLIP_NONE);
			posicionAuxiliar = capaNubes.x;

			//Renderizo la porción izquierda de la textura entrante, y la posiciono a continuación de la textura renderizada arriba
			capaNubes.w = ANCHO_VENTANA - (texturaFondoEscenarioCapaCuatro->getAncho() - capaNubes.x);
			capaNubes.x = 0;
			texturaFondoEscenarioCapaCuatro->aplicarPosicionDePorcion((ANCHO_VENTANA - capaNubes.w),0,&capaNubes,0,SDL_FLIP_NONE);
			capaNubes.x = posicionAuxiliar;

			//Si la textura saliente salió por completo de la pantalla, reseteo todos los valores
			if (capaNubes.x > texturaFondoEscenarioCapaCuatro->getAncho()){
				capaNubes.x = 0;
				velocidad_nubes = 0;
				capaNubes.w = ANCHO_VENTANA;
			}
		} else {
			texturaFondoEscenarioCapaCuatro->aplicarPosicionDePorcion(0,0,&capaNubes,0,SDL_FLIP_NONE);
		}

		//Establezco la capa arena
		capaTresArena.y = camara.y;

		/* Paralaje infinito de las arenas */
		//Con esta ecuación establezco la velocidad de las capas para que todas terminen alineadas al término del escenario
		capaTresArena.x = abs((velocidad_capaTres_X)*(texturaFondoEscenarioCapaTres->getAncho() - ANCHO_VENTANA)/(anchoEscenario - ANCHO_VENTANA));

		//Si la textura comienza a salirse de pantalla
		if((capaTresArena.x + ANCHO_VENTANA) > texturaFondoEscenarioCapaTres->getAncho()){
			//Creo una posición auxiliar para no perder el valor de capaTresArena.x
			int posicionAuxiliar = 0;

			capaTresArena.w = texturaFondoEscenarioCapaTres->getAncho() - capaTresArena.x;
			texturaFondoEscenarioCapaTres->aplicarPosicionDePorcion(0,0,&capaTresArena,0,SDL_FLIP_NONE);
			posicionAuxiliar = capaTresArena.x;

			//Renderizo la porción izquierda de la textura entrante, y la posiciono a continuación de la textura renderizada arriba
			capaTresArena.w = ANCHO_VENTANA - (texturaFondoEscenarioCapaTres->getAncho() - capaTresArena.x);
			capaTresArena.x = 0;
			texturaFondoEscenarioCapaTres->aplicarPosicionDePorcion((ANCHO_VENTANA - capaTresArena.w),0,&capaTresArena,0,SDL_FLIP_NONE);
			capaTresArena.x = posicionAuxiliar;

			//Si la textura saliente salió por completo de la pantalla, reseteo todos los valores
			if (capaTresArena.x > texturaFondoEscenarioCapaTres->getAncho()){
				velocidad_capaTres_X = 0;
				capaTresArena.w = ANCHO_VENTANA;
			}
		} else {
			texturaFondoEscenarioCapaTres->aplicarPosicionDePorcion(0,0,&capaTresArena,0,SDL_FLIP_NONE);
		}


		capaDosArena.x = abs((velocidad_capaDos_X)*(texturaFondoEscenarioCapaDos->getAncho() - ANCHO_VENTANA)/(anchoEscenario - ANCHO_VENTANA));

		if((capaDosArena.x + ANCHO_VENTANA) > texturaFondoEscenarioCapaDos->getAncho()){
			//Creo una posición auxiliar para no perder el valor de capaDosArena.x
			int posicionAuxiliar = 0;

			capaDosArena.w = texturaFondoEscenarioCapaDos->getAncho() - capaDosArena.x;
			texturaFondoEscenarioCapaDos->aplicarPosicionDePorcion(0,0,&capaDosArena,0,SDL_FLIP_NONE);
			posicionAuxiliar = capaDosArena.x;

			//Renderizo la porción izquierda de la textura entrante, y la posiciono a continuación de la textura renderizada arriba
			capaDosArena.w = ANCHO_VENTANA - (texturaFondoEscenarioCapaDos->getAncho() - capaDosArena.x);
			capaDosArena.x = 0;
			texturaFondoEscenarioCapaDos->aplicarPosicionDePorcion((ANCHO_VENTANA - capaDosArena.w),0,&capaDosArena,0,SDL_FLIP_NONE);
			capaDosArena.x = posicionAuxiliar;

			//Si la textura saliente salió por completo de la pantalla, reseteo todos los valores
			if (capaDosArena.x > texturaFondoEscenarioCapaDos->getAncho()){
				velocidad_capaDos_X = 0;
				capaDosArena.w = ANCHO_VENTANA;
			}
		} else {
			texturaFondoEscenarioCapaDos->aplicarPosicionDePorcion(0,0,&capaDosArena,0,SDL_FLIP_NONE);
		}


		//Establezco la primera capa y el jugador
		capaCiudad.x = camara.x;
		capaCiudad.y = camara.y;

		/* Paralaje infinito de la ciudad */
		//Si la 1era textura comienza a salirse de pantalla
		if((capaCiudad.x + ANCHO_VENTANA) > texturaFondoEscenarioCapaUno->getAncho()){
			//Y si el rectángulo está renderizando entre la 1era textura y la 2da textura (reflejada)
			if(capaCiudad.x < texturaFondoEscenarioCapaUno->getAncho()){
				//Renderizo la porción derecha de la 1er textura y la posiciono en x=0
				capaCiudad.w = texturaFondoEscenarioCapaUno->getAncho() - capaCiudad.x;
				texturaFondoEscenarioCapaUno->aplicarPosicionDePorcion(0,0,&capaCiudad,0,SDL_FLIP_NONE);

				//Renderizo la porción izquierda de la 2da textura (reflejada), y la posiciono a continuación de la textura renderizada arriba
				//Tener en cuenta que reflejar una textura con SDL_FLIP_HORIZONTAL implica también reflejar las posiciones y dimensiones aplicadas sobre el mismo
				capaCiudad.w = ANCHO_VENTANA - (texturaFondoEscenarioCapaUno->getAncho() - capaCiudad.x);
				capaCiudad.x = (texturaFondoEscenarioCapaUno->getAncho() - capaCiudad.w);
				texturaFondoEscenarioCapaUno->aplicarPosicionDePorcion((ANCHO_VENTANA - capaCiudad.w),0,&capaCiudad,0,SDL_FLIP_HORIZONTAL);
			} else {
				/* Si acá el rectángulo entró a renderizar por completo la 2da textura (reflejada), donde la 1era textura ya salió por completo de pantalla,
				   renderizo el ancho completo de la ventana ya que estoy en la 2da textura */
				capaCiudad.w = ANCHO_VENTANA;
				//A medida que avanzo, capaCiudad.x va disminuyendo (en la textura reflejada disminuye de izquierda a derecha)
				capaCiudad.x = (texturaFondoEscenarioCapaUno->getAncho() - ANCHO_VENTANA) - (capaCiudad.x - texturaFondoEscenarioCapaUno->getAncho());

				// Si la 2da textura (reflejada) comienza a salirse de pantalla
				if(capaCiudad.x < 0){
					//Renderizo la porción derecha de la 2da textura y la posiciono en x=0
					capaCiudad.w = ANCHO_VENTANA + capaCiudad.x;
					capaCiudad.x = 0;
					texturaFondoEscenarioCapaUno->aplicarPosicionDePorcion(0,0,&capaCiudad,0,SDL_FLIP_HORIZONTAL);

					//Renderizo la porción izquierda de la 1era textura, y la posiciono a continuación de la 2da textura renderizada arriba
					capaCiudad.w = ANCHO_VENTANA - (capaCiudad.w);
					texturaFondoEscenarioCapaUno->aplicarPosicionDePorcion((ANCHO_VENTANA - capaCiudad.w),0,&capaCiudad,0,SDL_FLIP_NONE);

					//Si la 2da textura salió por completo de pantalla y se renderiza por completo la 1era textura, reseteo todas las variables
					if (capaCiudad.w > ANCHO_VENTANA){
						camara.x = 0;
						capaCiudad.w = ANCHO_VENTANA;
						capaCiudad.x = 0;
						jugador_X = ANCHO_VENTANA/2 - texturaJugador->getAnchoSprite()/2;
					}
				} else {
					texturaFondoEscenarioCapaUno->aplicarPosicionDePorcion(0,0,&capaCiudad,0,SDL_FLIP_HORIZONTAL);
				}
			}
		} else {
			capaCiudad.w = ANCHO_VENTANA;
			texturaFondoEscenarioCapaUno->aplicarPosicionDePorcion(0,0,&capaCiudad,0,SDL_FLIP_NONE);
		}


		texturaJugador->aplicarPosicion(jugador_X-camara.x,jugador_Y-camara.y,0,flip);
		this->ventana->actualizar();
	}
}

bool Vista::ventanaCerrada(){
	return controlador->comprobarCierreVentana();
}

void Vista::cerrar(){
	this->ventana->cerrar();
}

