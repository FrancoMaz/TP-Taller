/*
 * Servidor.h
 *
 *  Created on: 28 ago. 2016
 *      Author: franco
 */

#ifndef TP1_SERVIDOR_H_
#define TP1_SERVIDOR_H_

#include <string>
#include <list>
#include <queue>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include "Cliente.h"
#include "Mensaje.h"
#include "Log.h"
using namespace std;

class Servidor {

private:
	int puerto;
	int socket;
	string nombreArchivo;
	FILE* archivoUsuarios;
	queue<Mensaje> colaMensajesNoProcesados;
	list<Mensaje> listaMensajesProcesados;
	Log archivoLog;
	pthread_t threadProcesador;
	pthread_t threadChecker;

public:
	Servidor();
	Servidor(int puerto);
	Servidor(string nombreArchivoDeUsuarios, int puerto);
	virtual ~Servidor();
	bool escuchando = false; //representa si el servidor esta disponible para escuchar pedidos
	list<Cliente> autenticar(string nombre, string contrasenia);
	list<Cliente> obtenerClientes();
	void guardarLog();
	list<Mensaje> obtenerMensajes(Cliente cliente);
	void crearMensaje(Mensaje mensaje);
	void comenzarEscucha();
	void finalizarEscucha();
	queue<Mensaje> getColaMensajesNoProcesados();
};

#endif /* TP1_SERVIDOR_H_ */
