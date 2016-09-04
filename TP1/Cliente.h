/*
 * Cliente.h
 *
 *  Created on: 28 ago. 2016
 *      Author: franco
 */

#ifndef TP1_CLIENTE_H_
#define TP1_CLIENTE_H_

#include <string>
#include <pthread.h>
#include <list>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <iterator>
#include "Mensaje.h"

using namespace std;

class Cliente {
private:
	int opcionMenu;
	char* direccionIP;
	int puertoServidor;
	string nombre;
	int socketCliente;
	pthread_t threadComunicacion;
	pthread_t threadProcesador;
	pthread_t threadChecker;
	struct sockaddr_in direccionServidor;
	list<Cliente> clientesDisponibles;

public:
	Cliente();
	Cliente(char* ip, int puerto);
	virtual ~Cliente();
	void mostrarMenu();
	int inicializarThreadConexion();
	void elegirOpcionDelMenu(int opcion);
	list<Cliente> conectar(string nombre, string contrasenia);
	void desconectar();
	void salir();
	void enviar(string mensaje, string destinatario);
	list<string> recibir();
	void loremIpsum(int frecuenciaDeEnvios,int cantidadMaximaDeEnvios);
	string getNombre();
	int getOpcionMenu();
	void* cicloConexion(void* arg);
	list<Cliente> getClientesDisponibles();
};

#endif /* TP1_CLIENTE_H_ */
