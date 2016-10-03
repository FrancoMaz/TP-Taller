/*
 * Controlador.h
 *
 *  Created on: 1 oct. 2016
 *      Author: pablo
 */

#ifndef TP2_CONTROLADOR_H_
#define TP2_CONTROLADOR_H_

#include <SDL2/SDL.h>
#include <string>

using namespace std;

class Controlador {
private:
	SDL_Event evento;
	bool botonCerrarVentana;
	const Uint8* teclaEscaneada;

	bool aplicarEvento(SDL_Keycode boton, Uint32 tipo);

public:
	Controlador();
	~Controlador();

	//Comprueba si se presiono un botón (pasado por argumento) y devuelve true si está presionado
	bool presionarBoton(SDL_Keycode boton);

	//Idem arriba, pero si el botón se suelta
	bool soltarBoton(SDL_Keycode boton);

	//Escanea el boton que está siendo presionado, generalmente es bueno para el cambio de texturas
	bool escanearBoton(SDL_Scancode boton);

	//Comprueba si se presionó el boton X de cerrar ventana
	void botonCerrarVentanaSeleccionado();

	/*
	Idem pero se fija si puede cerrar la ventana por cada bucle según lo establecido por el método de arriba.
	El bucle while de cada pantalla depende del valor devuelto por este método. Por lo tanto, si usamos el
	método de arriba cuando presionamos el X, los siguientes bucles correspondientes a las siguientes pantallas
	no se ejecutaran, yendo directo al return del main.
	*/
	bool comprobarCierreVentana();

	//Este método puede ser invocado por otras funciones que requieran salir del juego, por ejemplo el botón Salir de menú
	void setCerrarVentana();

	//A partir del texto pasado por argumento, lo va actualizando agregando caracteres que se ingresan por teclado
	void ingresarCaracteresATexto(string* textoParaActualizar);

	SDL_Event obtenerEvento();
};

#endif /* TP2_CONTROLADOR_H_ */
