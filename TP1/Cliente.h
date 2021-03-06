/*
 * Cliente.h
 *
 *  Created on: 28 ago. 2016
 *      Author: franco
 */

#ifndef TP1_CLIENTE_H_
#define TP1_CLIENTE_H_

#include <string>
#include <string.h>
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
#define BUFFER_MAX_SIZE 200
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
	bool terminoComunicacion;

public:
	Cliente();
	Cliente(string ip, int puerto);
	virtual ~Cliente();
	void mostrarMenuYProcesarOpcion();
	void elegirOpcionDelMenu(int opcion);
	void conectar(string nombre, string contrasenia);
	void desconectar();
	void salir();
	void enviar(string mensaje, string destinatario);
	void recibir();
	void loremIpsum(double frecuenciaDeEnvios,double cantidadMaximaDeEnvios);
	string getNombre();
	int getOpcionMenu();
	void setOpcionMenu(int opcion);
	list<string> getClientesDisponibles();
	void vaciarClientesDisponibles();
	pthread_t getThreadComunicacion();
	void setThreadComunicacion(pthread_t thrComu);
	void splitUsuarios(string datosRecibidos);
	void mostrarClientesDisponibles();
	void mostrarUltimosMensajes(string colaMensajes);
	string devolverNombre(int numeroDestinatario);
	void enviarMensajeATodos(string mensaje);
	void inicializarSocket();
	void corroborarConexion();
	bool getTermino();
	bool stringTerminaCon(std::string const &fullString, std::string const &ending);
};

#endif /* TP1_CLIENTE_H_ */

