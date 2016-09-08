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
#include <queue>
#define BUFFER_MAX_SIZE 1024
using namespace std;

class Cliente {
private:
	int opcionMenu;
	char* direccionIP;
	int puertoServidor;
	string nombre;
	int socketCliente;
	struct sockaddr_in direccionServidor;
	socklen_t addr_size;
	pthread_t threadComunicacion;
	pthread_t threadProcesador;
	pthread_t threadChecker;
	list<string> clientesDisponibles;

public:
	Cliente();
	Cliente(char* ip, int puerto);
	virtual ~Cliente();
	void mostrarMenuYProcesarOpcion();
	void elegirOpcionDelMenu(int opcion);
	list<string> conectar(string nombre, string contrasenia);
	void desconectar();
	void salir();
	void enviar(string mensaje, string destinatario);
	queue<Mensaje> recibir();
	void loremIpsum(double frecuenciaDeEnvios,double cantidadMaximaDeEnvios);
	string getNombre();
	int getOpcionMenu();
	list<string> getClientesDisponibles();
	void setClientesDisponibles(string nombre, string contrasenia);
	pthread_t getThreadComunicacion();
	void setThreadComunicacion(pthread_t thrComu);
};

#endif /* TP1_CLIENTE_H_ */
