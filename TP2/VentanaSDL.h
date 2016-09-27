/*
 * VentanaSDL.h
 *
 *  Created on: 18 sep. 2016
 *      Author: pablo
 */

#ifndef TP2_VENTANASDL_H_
#define TP2_VENTANASDL_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

using namespace std;

//Dimensiones de la ventana
const int ANCHO_VENTANA = 800;
const int ALTO_VENTANA = 600;

class VentanaSDL {

private:
	SDL_Window* ventana;
	SDL_Renderer* renderizacion;
	SDL_Texture* textura;

public:
	VentanaSDL();
	~VentanaSDL();
	bool inicializar();
	bool cargarImagen(string rutaDeArchivo);
	void actualizar();
	void cerrar();
};

#endif /* TP2_VENTANASDL_H_ */
