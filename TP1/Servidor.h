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
using namespace std;

class Servidor {
public:
	Servidor();
	Servidor(int puerto);
	Servidor(string nombreArchivoDeUsuarios, int puerto);
	virtual ~Servidor();
	bool escuchando = false; //representa si el servidor esta disponible para escuchar pedidos
	list<Cliente> autenticar(string nombre, string contrasenia);
	list<Cliente> obtenerClientes();
	void guardarLog();
	list<string> obtenerMensajes(Cliente cliente);
	void crearMensaje(Cliente clienteOrigen, Cliente clienteDestinatario);
	void comenzarEscucha();
	void finalizarEscucha();

private:
	int puerto;
	int socket;
	ArchivoUsuario archivoUsuarios;
	queue<string> colaMensajesNoProcesados;
	TablaDeHash hashMensajesProcesados;
	Log archivoLog;
	pthread_t threadProcesador;
	pthread_t threadChecker;
	TablaDeHash hashThreadsClientes;
};

#endif /* TP1_SERVIDOR_H_ */
