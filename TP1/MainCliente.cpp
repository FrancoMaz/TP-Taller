/*
 * mainCliente.cpp
 *
 *  Created on: Aug 30, 2016
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
#include "Cliente.h"

using namespace std;

bool chequearSocket(string ip, int puerto)
{
	return true;
}

void* cicloConexion(void* arg)
{
	Cliente cliente = *(Cliente*)arg;
	while(cliente.getOpcionMenu() != 5 and cliente.getOpcionMenu() != 4) //mientras la opcion del menu no sea salir o desconectar..
	{
		cliente.mostrarMenuYProcesarOpcion();
	}
	return NULL;
}

int main()
{
	char* ip;
	int puerto, accion;
	bool socketOk = false;
	while(!socketOk)
	{
		cout << "Ingrese el puerto para la conexion: ";
		cin >> puerto;
		cout << "Ingrese la ip del servidor:";
		cin >> ip;
		socketOk = chequearSocket(ip, puerto); //FALTA IMPLEMENTAR METODO DE CHEQUEAR IP/PUERTO. ESTA MAS ABAJO LA FUNCION.
		if (!socketOk)
		{
			//SI NO SE PUDO COMPROBAR LA CONEXION, DOY ERROR Y VUELVO A PEDIR IP Y CONEXION
			cout << "Error: La direccion de ip o el puerto no permiten esta conexion." << endl;
		}
	}
	cout << "SOCKET OK" << endl;
	Cliente *cliente = new Cliente(ip, puerto);
	string nombre, contrasenia;
	cout << "SISTEMA DE MENSAJERIA" << endl;
	while (accion < 1 or accion > 2)
	{
		cout << "1) Conectar" << endl;
		cout << "2) Salir" << endl;
		cout << "Que desea hacer? " << endl;
		cin >> accion;
	}
	if (accion == 2)
	{
		//si la accion es salir, devuelvo 0 y salgo del programa.
		return 0;
	}
	//si no es salir, intento conectar pidiendo usuario y contraseña.
	/*while (cliente->getClientesDisponibles().empty())
	{
		cout << "Ingrese nombre de usuario: ";
		cin >> nombre;
		cout << "Ingrese contrasenia: ";
		cin >> contrasenia;
		cliente->setClientesDisponibles(nombre,contrasenia);
		if (cliente->getClientesDisponibles().empty())
		{
			//si despues de intentar conectar, sigue siendo una lista vacia, muestro error y pido nuevamente.
			cout << "Error al intentar autenticar. Ingrese un nombre de usuario y una contrasenia validos." << endl;
		}
	}*/
	string opc = "";
	while (opc == "")
	{
		cout << "hola" << endl;
		cin >> opc;
	}
	//cout << "Autenticación OK. Bienvenido al sistema de mensajería. ¿Qué acción desea realizar?" << endl;
	//cliente->mostrarMenu();
	pthread_t thrComu = cliente->getThreadComunicacion();
	int threadOk = pthread_create(&thrComu,NULL, &cicloConexion,cliente );
	if (threadOk != 0)
	{
		cout << "Error al inicializar la conexion." << endl;
	}
	else{
		cout << "Thread comu ok" << endl;
		pthread_join(cliente->getThreadComunicacion(), NULL);
	}
	return 0;
}
