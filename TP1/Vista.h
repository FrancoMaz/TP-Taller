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
	Controlador* controlador;
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

public:
	Vista();
	~Vista();
	void cargarPrimeraPantalla();
	datosConexion cargarSegundaPantalla(bool aviso);
	datosConexion cargarTerceraPantalla(bool aviso);
	void cargarEscenario(vector<ImagenDto*> imagenes, int anchoVentana, int altoVentana);
	void transicionDePantalla();
	bool inicializar();
	void cargarArchivos();
	bool ventanaCerrada();
	void cerrar();
	void actualizarJugador(UpdateJugador* update, int anchoVentana);
	void cargarVistaInicialJugador(string nombre, int x, int y, string sprite);
	void actualizarCamara(int x, int y, int vel, int anchoVentana);
	SDL_Rect camara;

};

#endif /* TP2_VISTA_H_ */
