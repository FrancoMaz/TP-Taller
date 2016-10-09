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

using namespace std;

class Jugador {
private:
	pair<int,int> posicion;
	string nombre;

public:
	Jugador(pair<int,int> posicionInicial);
	Jugador(string nombre);
	virtual ~Jugador();
	void actualizarPosicion(pair<int,int> nuevaPosicion);
	string getStringPosicion();
	string getNombre();

};

#endif /* JUGADOR_H_ */
