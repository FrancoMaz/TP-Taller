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

using namespace std;

class Cliente {
private:
	int opcionMenu;
	string direccionIP;
	int puertoServidor;
	string nombre;
	int socketCliente;
	pthread_t threadComunicacion;
	pthread_t threadProcesador;
	pthread_t threadChecker;
	struct sockaddr_in direccionServidor;

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
};

#endif /* TP1_CLIENTE_H_ */
