/*
 * Servidor.h
 *
 *  Created on: 28 ago. 2016
 *      Author: franco
 */

#ifndef TP1_SERVIDOR_H_
#define TP1_SERVIDOR_H_

#include <string>
using namespace std;

class Servidor {
public:
	Servidor();
	virtual ~Servidor();
	List<Cliente> autenticar(string nombre, string contrasenia);
	List<Cliente> obtenerClientes();
	void guardarLog();
	List<string> obtenerMensajes(Cliente cliente);
	void crearMensaje(Cliente clienteOrigen, Cliente clienteDestinatario);

private:
	int puerto;
	Socket socket;
	ArchivoUsuario archivoUsuarios;
	Cola colaMensajesNoProcesados;
	TablaDeHash hashMensajesNoProcesados;
	Log archivoLog;
	Thread threadProcesador;
	Thread threadChecker;
	TablaDeHash hashThreadsClientes;
};

#endif /* TP1_SERVIDOR_H_ */
