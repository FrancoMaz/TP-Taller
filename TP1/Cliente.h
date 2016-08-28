/*
 * Cliente.h
 *
 *  Created on: 28 ago. 2016
 *      Author: franco
 */

#ifndef TP1_CLIENTE_H_
#define TP1_CLIENTE_H_

#include <string>
using namespace std;

class Cliente {
public:
	Cliente();
	virtual ~Cliente();
	void mostrarMenu();
	void elegirOpcionDelMenu(int opcion);
	List<Cliente> conectar(string nombre, string contrasenia);
	void desconectar();
	void salir();
	void enviar(string mensaje, string destinatario);
	List<string> recibir();
	loremIpsum(int frecuenciaDeEnvios,int cantidadMaximaDeEnvios);

private:
	int opcionMenu;
	string direccionIP;
	int puertoServidor;
	string nombre;
	Socket socket;
	Thread threadComunicacion;
	Thread threadProcesador;
	Thread threadChecker;
};

#endif /* TP1_CLIENTE_H_ */
