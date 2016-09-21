/*
 * TexturaSDL.h
 *
 *  Created on: 21 sep. 2016
 *      Author: pablo
 */

#ifndef TP2_TEXTURASDL_H_
#define TP2_TEXTURASDL_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

using namespace std;

class TexturaSDL {

private:
	SDL_Texture* textura;
	SDL_Renderer* render;
	int ancho;
	int alto;
	float posX;
	float posY;

public:
	TexturaSDL(SDL_Renderer* renderer);
	~TexturaSDL();

	//Elimina las referencias de la textura, liberando memoria
	void limpiar();

	//Carga una imagen de una ruta específica
	bool cargarImagen(string ruta);

	//Coloca la textura en una posición de la pantalla
	void aplicarPosicion(float x, float y);

	//Obtener las dimensiones
	int getAncho();
	int getAlto();

	//Obtener las posiciones
	float getPosX();
	float getPosY();
};

#endif /* TP2_TEXTURASDL_H_ */
