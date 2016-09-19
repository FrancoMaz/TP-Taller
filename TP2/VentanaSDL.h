/*
 * VentanaSDL.h
 *
 *  Created on: 18 sep. 2016
 *      Author: pablo
 */

#ifndef TP2_VENTANASDL_H_
#define TP2_VENTANASDL_H_

#include <SDL2/SDL.h>

using namespace std;

//Dimensiones de la ventana
const int ANCHO_VENTANA = 800;
const int ALTO_VENTANA = 600;

class VentanaSDL {

private:
	SDL_Window* ventana;
	SDL_Surface* capaPrincipal;
	SDL_Surface* imagenDeEspera;

public:
	VentanaSDL();
	~VentanaSDL();
	bool inicializar();
	bool cargarImagen(const char* rutaDeArchivo);
	void actualizar();
};

#endif /* TP2_VENTANASDL_H_ */
