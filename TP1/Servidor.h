/*
 * Servidor.h
 *
 *  Created on: 28 ago. 2016
 *      Author: franco
 */

#ifndef TP1_SERVIDOR_H_
#define TP1_SERVIDOR_H_

#include <string>
#include <iostream>
#include <list>
#include <queue>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Cliente.h"
#include "Mensaje.h"
#include "Log.h"
using namespace std;

class Servidor {

private:
	int puerto;
	int welcomeSocket;
	char* nombreArchivo;
	queue<Mensaje> colaMensajesNoProcesados;
	list<Mensaje> listaMensajesProcesados;
	Log archivoLog;
	struct sockaddr_in serverAddr;
	struct sockaddr_storage serverStorage;
	socklen_t addr_size;
	pthread_t threadProcesador;
	pthread_t threadChecker;

public:
	Servidor();
	Servidor(char* nombreArchivoDeUsuarios, int puerto);
	virtual ~Servidor();
	bool escuchando = false; //representa si el servidor esta disponible para escuchar pedidos
	list<string>* autenticar(string nombre, string contrasenia);
	list<Cliente> obtenerClientes();
	void guardarLog();
	list<Mensaje> obtenerMensajes(Cliente cliente);
	void crearMensaje(Mensaje mensaje);
	void comenzarEscucha();
	void finalizarEscucha();
	queue<Mensaje> getColaMensajesNoProcesados();
};

#endif /* TP1_SERVIDOR_H_ */
