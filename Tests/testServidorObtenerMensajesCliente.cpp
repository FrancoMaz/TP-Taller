/*
 * testServidorObtenerMensajesCliente.cpp
 *
 *  Created on: Sep 1, 2016
 *      Author: franco
 */




#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>

using namespace std;
int main()
{
	//TEST DE LA FUNCIONALIDAD DE DEVOLVER UN MENSAJE PARA UN CLIENTE
	Servidor servidor = new Servidor();
	Cliente cliente = new Cliente();
	cliente.nombre = "fulano";
	Cliente cliente2 = new Cliente();
	cliente.nombre = "mengano";
	servidor.crearMensaje(cliente,cliente2,"Hola como te va?");
	list<Mensaje> mensajes = servidor.obtenerMensajes(cliente2); //implementar clase mensaje con destinatario, remitente y texto del mensaje.
	cout << (mensajes.size == 1) << endl;
	return 0;
}
