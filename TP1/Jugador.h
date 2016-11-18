/*
 * Jugador.h
 *
 *  Created on: 8 oct. 2016
 *      Author: franco
 */

#ifndef JUGADOR_H_
#define JUGADOR_H_

#include <utility>
#include <string>
#include <iostream>
#include <SDL2/SDL.h>
#include "VentanaSDL.h"
#include "TexturaSDL.h"
#include "Personaje.h"
#include "HeavyMachineGun.h"
#include "RocketLauncher.h"
#include "Flameshot.h"

#define velocidad 3
#define PI 3.14159265
#define VELMAX 10
#define MARGENIZQ 15
#define MARGENDER ANCHO_VENTANA/2
#define DER 1
#define IZQ 2
#define ARRIBA 3
#define ABAJO 4
#define ESPACIO 5
#define PISO 415
#define MARGENBOXCOLLIDER 87
#define PLATAFORMA 248
#define GRAVEDAD 10
using namespace std;

class Jugador: public Personaje {
private:
	pair<int,int> posicion;
	pair<int,int> velocidades;
	string nombre;
	string equipo;
	bool saltar;
	bool agachar;
	bool disparar;
	bool caer;
	bool movDerecha = false;
	bool movIzquierda = false;
	double angulo;
	string spriteAEjecutar;
	bool conectado = false;
	string condicionSprite;
	pthread_t thrMov;
	SDL_Keycode ultimaTeclaPresionada = SDLK_RIGHT;
	vector<pair<int,int>> vectorPlataforma;
	SDL_Rect boxCollider;
	int armaActual;
public:
	Jugador(string nombre, string equipo, int posicionX, vector<pair<int,int>>);
	virtual ~Jugador();
	void actualizarPosicion(SDL_Keycode tecla, bool sePresionoTecla, SDL_Rect camara);
	string getStringJugador();
	string getNombre();
	string getSpriteAEjecutar();
	bool salto();
	void setDesconectado();
	void setConectado();
	bool getConectado();
	string serializarInicio();
	bool chequearCambiarCamara(SDL_Rect camara, int anchoVentana, pair<int,int> posicionesExtremos, int anchoSprite);
	pair<int, int> getPosicion();
	int getVelocidadX();
	void resetearPosicion(int anchoCapaPrincipal);
	void setPosicion(int posicion);
	void setSprite(string sprite, bool equipo=false,string condicion = "");
	string getStringJugadorDesconectado();
	string getEquipo();
	void setPosicion(int x, int y);
	void setMov(SDL_Keycode tecla, bool estaMoviendose);
	vector<bool> getMov();
	void mover(SDL_Rect camara);
	pthread_t getThreadMovimiento();
	void setThreadMovimiento(pthread_t thrMovimiento);
	bool esPlataforma(int x);
	void inicializarVectorArmas();
	void dispararProyectil();
};
#endif /* JUGADOR_H_ */
