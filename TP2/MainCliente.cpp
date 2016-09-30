/*
 * mainCliente.cpp
 *
 *  Created on: Aug 30, 2016
 *      Author: franco
 */

#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include "Cliente.h"
#include "VentanaSDL.h"
#include "TexturaSDL.h"
#include <SDL2/SDL.h>

using namespace std;

bool chequearSocket(string ip, int puerto) {
	//string ipServer = "192.168.1.10";
	string ipServer = "127.0.0.1";
	int puertoDeEscucha = 7891;

	return (ip == ipServer && puerto == puertoDeEscucha);
}

void* cicloConexion(void* arg) {
	//Funcion que cicla para las opciones del cliente dentro del thread de comunicacion. Devuelve 1 si la opcion es desconectar, 0 si es salir.
	Cliente* cliente = (Cliente*) arg;
	string user, pass;
	while (cliente->getClientesDisponibles().empty()) {
		cout << "Ingrese nombre de usuario: ";
		cin >> user;
		cout << "Ingrese password: ";
		cin >> pass;
		cliente->conectar(user, pass);
	}
	do
	{
		cliente->mostrarMenuYProcesarOpcion();
	} while (cliente->getOpcionMenu() != 5 and cliente->getOpcionMenu() != 4); //mientras la opcion del menu no sea salir o desconectar..
	if (cliente->getOpcionMenu() == 4) {
		return (void*) 1;
	}
	return (void*) 0;
}

int main() {
	bool esValido = false;
	string ip;
	int puerto, accion;
	bool socketOk = false;
	pthread_t thrComu;
	string nombre, contrasenia;
	VentanaSDL* ventana = new VentanaSDL();

	if (!ventana->inicializar()){
		cout << "El programa no pudo ejecutarse." << endl;
	} else {
		TexturaSDL* texturaMenuFondo = ventana->crearTextura("Recursos/Menu_fondo.png",0);
		TexturaSDL* texturaMenuMetalSlug = ventana->crearTextura("Recursos/Menu_Metal_Slug.png",0);
		TexturaSDL* texturaMenuPressEnter = ventana->crearTextura("Recursos/Menu_Presionar_Enter.png",0);
		TexturaSDL* texturaEfectoLuz = ventana->crearTextura("Recursos/Efecto_luz.png",0);

		TexturaSDL* texturaBotonSalir = ventana->crearBoton("Recursos/Boton_Salir.png");
		TexturaSDL* texturaBotonCreditos = ventana->crearBoton("Recursos/Boton_Creditos.png");
		TexturaSDL* texturaBotonIngresar = ventana->crearBoton("Recursos/Boton_Ingresar.png");

		TexturaSDL* texturaCampoUno = ventana->crearTextura("Recursos/Efecto_luz.png",0);
		TexturaSDL* texturaCampoDos = ventana->crearTextura("Recursos/Efecto_luz.png",0);

		TexturaSDL* textoPuerto = ventana->crearTexto("Recursos/arial.ttf",14);
		TexturaSDL* textoIP = ventana->crearTexto("Recursos/arial.ttf",14);

		TexturaSDL* textoTPTaller = ventana->crearTexto("Recursos/arial.ttf",13);
		if (!ventana->comprobarTexturasCargadas()){
			cout << "El programa no pudo ejecutarse." << endl;
		} else {
			bool salir = false;			//Indica si se presiono el botón X de la venana
			bool siguiente = false;		//Indica si se puede pasar a la siguiente pantalla
			const Uint8* teclaActual = SDL_GetKeyboardState( NULL );	//Esta variable captura la tecla que se está presionando
			SDL_Event e;

			//Primera pantalla (Presionar Enter)

			for (float y=-290; y<40; y=y+5){
				ventana->limpiar();
				texturaMenuFondo->aplicarPosicion(0,0,0,SDL_FLIP_NONE);
				texturaMenuMetalSlug->aplicarPosicion(180,y,0,SDL_FLIP_NONE);
				ventana->actualizar();
			}

			SDL_Color colorTexto = {255,255,255};
			textoTPTaller->actualizarTexto("Trabajo Práctico - Taller de Programación", colorTexto);

			for (float a= 255; a>0; a=a-3){
					ventana->limpiar();
					texturaMenuFondo->aplicarPosicion(0,0,0,SDL_FLIP_NONE);
					texturaMenuMetalSlug->aplicarPosicion(180,40,0,SDL_FLIP_NONE);
					texturaMenuPressEnter->aplicarPosicion(200,430,0,SDL_FLIP_NONE);
					textoTPTaller->aplicarPosicion(284,560,0,SDL_FLIP_NONE);
					textoTPTaller->setAlpha(200);
					texturaEfectoLuz->aplicarPosicionConTamanio(0,0,800,600);
					texturaEfectoLuz->setAlpha(a);
					ventana->actualizar();
			}

			bool incrementa = true;
			int a = 255;
			//Mientras la ventana no se cierre pulsando X o no se presione el enter, hacer el loop
			while ((!salir)&&(!siguiente)){
					ventana->limpiar();
					texturaMenuFondo->aplicarPosicion(0,0,0,SDL_FLIP_NONE);
					texturaMenuMetalSlug->aplicarPosicion(180,40,0,SDL_FLIP_NONE);
					texturaMenuPressEnter->aplicarPosicion(200,430,0,SDL_FLIP_NONE);
					if (a>=255)	incrementa = false;
					if (a<=0) incrementa = true;
					if (incrementa){
						a=a+3;
					} else {
						a=a-3;
					}
					texturaMenuPressEnter->setAlpha(a);
					textoTPTaller->aplicarPosicion(284,560,0,SDL_FLIP_NONE);
					textoTPTaller->setAlpha(200);
					ventana->actualizar();

					while(SDL_PollEvent(&e) != 0){
						if (e.type == SDL_QUIT){
							salir = true;
						} else {
							if (e.type == SDL_KEYDOWN){
								if (e.key.keysym.sym == SDLK_RETURN) siguiente = true;
							}
						}
					}
			}


			//Segunda Pantalla (ingresar puerto y dirección IP)

			textoPuerto->actualizarTexto("Ingrese el puerto:",colorTexto);
			textoIP->actualizarTexto("Ingrese la IP del servidor:",colorTexto);
			a = 230;
			siguiente = false;

			while ((!salir)&&(!siguiente)){
				ventana->limpiar();
				texturaMenuFondo->aplicarPosicion(0,0,0,SDL_FLIP_NONE);
				texturaMenuMetalSlug->aplicarPosicion(180,40,0,SDL_FLIP_NONE);

				textoPuerto->aplicarPosicion(310,337,0,SDL_FLIP_NONE);
/*
				if( teclaActual[ SDL_SCANCODE_RIGHT ] )	{
					texturaCampoUno->cargarImagen("Recursos/Boton_Creditos.png");
					texturaCampoUno->generarSprite(3);
				} else {
					texturaCampoUno->cargarImagen("Recursos/Efecto_luz.png");
					texturaCampoUno->generarSprite(0);
				}
*/
				texturaCampoUno->aplicarPosicionConTamanio(310,360,185,28);

				textoIP->aplicarPosicion(310,402,0,SDL_FLIP_NONE);
				texturaCampoDos->aplicarPosicionConTamanio(310,425,185,28);

				if (texturaBotonSalir->aplicarPosicionDeBoton(656,550,&e)){
					salir = true;
				}

				texturaBotonCreditos->aplicarPosicionDeBoton(30,550,&e);
				texturaBotonIngresar->aplicarPosicionDeBoton(338,480,&e);

				textoTPTaller->aplicarPosicion(284,560,0,SDL_FLIP_NONE);
				textoTPTaller->setAlpha(200);
				texturaEfectoLuz->aplicarPosicionConTamanio(0,0,800,600);
				texturaEfectoLuz->setAlpha(a);
				ventana->actualizar();

				while(SDL_PollEvent(&e) != 0){
					if (e.type == SDL_QUIT){
						salir = true;
					}
				}

				a = a-5;
				if (a <= 0) a = 0;
			}


			ventana->cerrar();

			/*
			while (!socketOk) {
				do {
					cout << "Ingrese el puerto para la conexion: ";
					cin >> puerto;
					if (cin.good()) {
						esValido = true;
					} else {
						cin.clear();
						cin.ignore();
						cout << "Error: el dato ingresado debe ser un numero" << endl;
					}
				} while (!esValido);

				cout << "Ingrese la ip del servidor: ";
				cin >> ip;
				socketOk = chequearSocket(ip, puerto); //FALTA IMPLEMENTAR METODO DE CHEQUEAR IP/PUERTO. ESTA MAS ABAJO LA FUNCION.
				if (!socketOk) {
					//SI NO SE PUDO COMPROBAR LA CONEXION, DOY ERROR Y VUELVO A PEDIR IP Y CONEXION
					cout << "Error: La direccion de ip o el puerto no permiten esta conexion." << endl;
				}
			}

			cout << "Socket OK" << endl;
			Cliente* cliente = new Cliente(ip, puerto);
			cout << "Bienvenido al sistema de mensajería" << endl;

			do {
				bool accionValida = false;
				cout << "1) Conectar" << endl;
				cout << "2) Salir" << endl;
				do {
					cout << "¿Qué desea hacer? " << endl;
					cin >> accion;
					if (cin.good() && (accion == 1 || accion == 2)) {
						accionValida = true;
					} else {
						cin.clear();
						cin.ignore();
						cout << "Error: la opcion ingresada no es valida" << endl;
					}
				} while (!accionValida);

				if (accion != 2) {
					//si no es salir, creo el thread de comunicacion que intenta conectar.
					int threadOk = pthread_create(&thrComu, NULL, &cicloConexion,cliente);
					if (threadOk != 0) {
						cout << "Error al inicializar la conexión." << endl;
					} else {
						cliente->setThreadComunicacion(thrComu);
						void** resultado;
						pthread_join(cliente->getThreadComunicacion(),(void**) &resultado); //espero que termine el thread de comunicacion que fue invocado..
						accion = *((int*) (&resultado));
						if (accion == 1) { //si es 1, es desconectar y vuelve a ingresar al loop que ofrece conectar y desconectar
							cout << "Desconectado del servidor.." << endl;
							cliente->vaciarClientesDisponibles();
						} //si es 0, va a salir automaticamente del loop y del programa.
					}
				} else {
					accion = 0; //si la accion es 2, la pongo en 0 para que salga del while
				}
			} while (accion != 0); //si la accion es 0, es salir.
			cliente->salir(); //cierra el socket y realiza trabajos de limpieza de memoria
			cout << "Saliendo del programa..." << endl;*/
		}
	}
	return 0;
}
