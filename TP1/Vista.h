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

public:
	Vista();
	~Vista();
	void cargarPrimeraPantalla();
	datosConexion cargarSegundaPantalla(bool aviso);
	datosConexion cargarTerceraPantalla(bool aviso);
	void cargarEscenario(int ancho, int alto);
	void transicionDePantalla();
	bool inicializar();
	void cargarArchivos();
	bool ventanaCerrada();
	void cerrar();
	void actualizarJugador(string remitente, int x, int y, string sprite, string condicion, int cantidadDeFotogramas);
	void cargarVistaInicialJugador(string nombre, int x, int y);

};

#endif /* TP2_VISTA_H_ */
