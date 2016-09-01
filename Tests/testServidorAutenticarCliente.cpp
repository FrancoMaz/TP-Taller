/*
 * testServidorAutenticarCliente.cpp
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
	//TEST DE LA FUNCIONALIDAD DEL AUTENTICAR DEL SERVIDOR. PRIMERO UN USER QUE PUEDE AUTENTICAR Y DESPUES UNO QUE DEVUELVA LA LISTA VACIA PORQE NO EXISTE ESA COMBINACION CLIENTE/PASS.
	//hay que hacer un archivo de texto con 6 clientes/passwords donde el primer nombre/pass este pero nombre2/pass2 no.
	string nombre = "fulano";
	string pass = "123456";
	Servidor servidor = new Servidor();
	list<Cliente> clientes = servidor.autenticar(nombre, pass);
	bool result = (clientes.size == 6);
	cout << result  << endl;
	string nombre2 = "fulanoError";
	string pass2 = "892159";
	list<Cliente> clientes = servidor.autenticar(nombre2, pass2);
	result = (clientes.size == 0);
	cout << result << endl;
	return 0;
}
