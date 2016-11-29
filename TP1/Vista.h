/*
 * Vista.h
 *
 *  Created on: 30 sep. 2016
 *      Author: pablo
 */

#ifndef TP2_VISTA_H_
#define TP2_VISTA_H_

#include "VentanaSDL.h"
#include "TexturaSDL.h"
#include "Controlador.h"
#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include "VistaJugador.h"
#include "UpdateJugador.h"
#include "Capa.h"
#include "Jugador.h"
#include "VistaBala.h"
#include "VistaEnemigo.h"
#include "Item.h"

#define PISO 301
#define PLATAFORMA 134

using namespace std;

struct datosConexion {
	string puerto;
	string ip;
	string nombre;
	string contrasenia;
};

class Vista {
private:
	VentanaSDL* ventana;
	vector<TexturaSDL*> textura;
	datosConexion datos;
	int opacidad;
	struct vistaJugador{
		TexturaSDL* texturaJugador;
		string nombre;
		int x;
		int y;
		string sprite;
	};
	vector<VistaJugador*> vistaJugadores;
	datosConexion cargarPantallaIngresoDatos(bool aviso, int numeroPantalla);
	vector<Capa*> vectorCapas;
	vector<VistaBala*> vistaBalas;
	vector<VistaEnemigo*> vistaEnemigos;
	vector<Item*> vistaItems;

public:
	Vista();
	~Vista();
	void cargarPrimeraPantalla();
	datosConexion cargarSegundaPantalla(bool aviso);
	datosConexion cargarTerceraPantalla(bool aviso);
	void cargarPantallaEsperandoJugadores();
	void cargarEscenario(vector<ImagenDto*> imagenes, int anchoVentana, int altoVentana);
	void transicionDePantalla();
	bool inicializar();
	void cargarArchivos();
	bool ventanaCerrada();
	void cerrar();
	void actualizarJugador(UpdateJugador* update, int anchoVentana, int anchoCapaPrincipal);
	void actualizarPosJugador(UpdateJugador* update, int anchoVentana, int anchoCapaPrincipal);
	void actualizarPantalla(int anchoVentana, int anchoCapaPrincipal);
	void cargarVistaInicialJugador(string nombre, int x, int y, SpriteDto* sprite);
	void actualizarCamara(int x, int y, vector<pair<int,int>> abscisasCapas, int anchoVentana);
	SDL_Rect camara;
	void inicializarCamara(int camaraX, int camaraY, int anchoVentana, int altoVentana, vector<pair<int,int>> abscisasCapas, vector<ImagenDto*> imagenes);
	void resetearVistas(int anchoCapaPrincipal);
	Controlador* controlador;
	bool salto;
	void actualizarEnemigo(string enemigo, int x, int y, string sprite, int id, int cantFotogramas);
	void actualizarProyectil(string nuevaBala, int x, int y, string sprite, int id, int cantFotogramas, string sentido, double angulo);
	void agregarVistaItem(string borrarItem, string sprite, int x, int y);
	void actualizarPuntaje(string jugador, int puntaje);
	void actualizarVida(string jugador, int vida);
};

#endif /* TP2_VISTA_H_ */
