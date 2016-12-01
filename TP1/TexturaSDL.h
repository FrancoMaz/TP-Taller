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
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

class TexturaSDL {

private:
	SDL_Texture* textura;
	SDL_Renderer* render;
	TTF_Font* fuente;
	vector<SDL_Rect> spriteClips;
	int frameActual;
	int ancho;
	int alto;
	int velocidadFrameDisminuida;

	void aplicarPosicionYTamanio(float x, float y, int ancho, int alto, SDL_Rect* clip, double rotacion, SDL_RendererFlip flip);
	void cargarImagenNoEncontrada();
	void cargarTextoPorDefecto(int tamanio);

public:
	TexturaSDL(SDL_Renderer* renderer);
	~TexturaSDL();

	//Elimina las referencias de la textura, liberando memoria
	void limpiarTextura();

	//Elimina las referencias de la fuente de texto, liberando memoria
	void limpiarFuente();

	//Carga una imagen de una ruta específica
	void cargarImagen(string ruta);

	//Carga una fuente de texto desde un archivo de fuente .ttf y su tamaño
	void cargarTexto(string ruta, int tamanio);

	//Setea el texto que se va a mostrar con su color
	bool actualizarTexto(string texto, SDL_Color color);

	//Coloca la textura en una posición de la pantalla con su respectiva rotación y reflejo (flip)
	void aplicarPosicion(float x, float y, double rotacion, SDL_RendererFlip flip);

	//Idem, pero dicha textura se ajusta al tamaño del ancho/alto
	void aplicarPosicionConTamanio(float x, float y, int ancho, int alto);

	//Idem aplicarPosicion pero colocando la textura de un fotograma específico para un sprite
	void aplicarPosicionDeFrame(float x, float y, int frame,double rotacion, SDL_RendererFlip flip);

	//Idem aplicarPosicion pero coloca la textura del botón según el evento del mouse
	//(por ejemplo, si pasa la fecha del mouse sobre el boton, debería cambiar su textura, o al hacer clic)
	bool aplicarPosicionDeBoton(float x, float y, SDL_Event* e);

	//Idem pero aplica una porción de textura definida por las dimensiones de clip
	void aplicarPosicionDePorcion(float x, float y, SDL_Rect* clip, double rotacion, SDL_RendererFlip flip);

	//Establecer transparencias (alpha = 0 transparente; alpha = 255 opaco)
	void setAlpha(Uint8 alpha);

	//Genera un sprite con la cantidad de fotogramas (frames) que se le pasa por argumento
	void generarSprite(int frames);

	//Resetea el fotograma del sprite a cero
	void frameReset();

	//Establece la velocidad del sprite (cuanto mayor sea el valor, menor será la velocidad de los fotogramas)
	void setVelocidadDisminuida(int valor);

	//Realiza una mezcla del color establecido por parámetro con los que hay en la textura
	void setColor(Uint8 rojo, Uint8 verde, Uint8 azul);

	//Obtengo el ancho y el alto
	int getAncho();
	int getAlto();

	//Obtengo el ancho y el alto de un sprite generado
	int getAnchoSprite();
	int getAltoSprite();
};

#endif /* TP2_TEXTURASDL_H_ */
