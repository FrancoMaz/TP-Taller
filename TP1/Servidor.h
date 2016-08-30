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
using namespace std;

class Servidor {
public:
	Servidor();
	virtual ~Servidor();
	list<Cliente> autenticar(string nombre, string contrasenia);
	list<Cliente> obtenerClientes();
	void guardarLog();
	list<string> obtenerMensajes(Cliente cliente);
	void crearMensaje(Cliente clienteOrigen, Cliente clienteDestinatario);

private:
	int puerto;
	int socket;
	ArchivoUsuario archivoUsuarios;
	Cola colaMensajesNoProcesados;
	TablaDeHash hashMensajesNoProcesados;
	Log archivoLog;
	pthread_t threadProcesador;
	pthread_t threadChecker;
	TablaDeHash hashThreadsClientes;
};

#endif /* TP1_SERVIDOR_H_ */
