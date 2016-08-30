/*
 * Cliente.h
 *
 *  Created on: 28 ago. 2016
 *      Author: franco
 */

#ifndef TP1_CLIENTE_H_
#define TP1_CLIENTE_H_

#include <string>
#include "pthread.h"
#include <list>
using namespace std;

class Cliente {
public:
	Cliente();
	Cliente(string ip, int puerto);
	virtual ~Cliente();
	void mostrarMenu();
	void elegirOpcionDelMenu(int opcion);
	list<Cliente> conectar(string nombre, string contrasenia);
	void desconectar();
	void salir();
	void enviar(string mensaje, string destinatario);
	list<string> recibir();
	void loremIpsum(int frecuenciaDeEnvios,int cantidadMaximaDeEnvios);

private:
	int opcionMenu;
	string direccionIP;
	int puertoServidor;
	string nombre;
	socket socket;
	pthread threadComunicacion;
	pthread threadProcesador;
	pthread threadChecker;
};

#endif /* TP1_CLIENTE_H_ */
