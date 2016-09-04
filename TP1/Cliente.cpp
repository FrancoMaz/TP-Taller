#include <stdlib.h>
#include "Cliente.h"
#include <iostream>
#include <string.h>

using namespace std;

Cliente::Cliente(string ip, int puerto) {
//El cliente se crea con la direccion IP y el puerto en el cual se encuentra disponible el servidor
	this->direccionIP = ip;
	this->puertoServidor = puerto;
	this -> socketCliente = socket(AF_INET, SOCK_STREAM, 0);
	direccionServidor.sin_family = AF_INET;
	direccionServidor.sin_port = htons(this -> puertoServidor);
	direccionServidor.sin_addr.s_addr = inet_addr((const char*) ip);
	memset(direccionServidor.sin_zero, '\0', sizeof direccionServidor.sin_zero);
	pthread_create(&(this->threadComunicacion), NULL, &cicloConexion,&this );
	this->mostrarMenu(); //Muestra el menu con todas las acciones que puede realizar el cliente
}

Cliente::~Cliente() {
	// TODO Auto-generated destructor stub
}

void* cicloConexion(void* arg)
{
	string nombre, contrasenia;
	list<Cliente> clientesDisponibles = NULL;
	Cliente cliente = *(Cliente*)arg;
	while (clientesDisponibles == NULL)
	{
		cout << "Ingrese nombre de usuario: ";
		cin >> nombre;
		cout << "Ingrese contrasenia: ";
		cin >> contrasenia;
		clientesDisponibles = cliente.conectar(nombre,contrasenia);
		if (clientesDisponibles == NULL)
		{
			cout << "Error al intentar autenticar. Ingrese un nombre de usuario y una contrasenia validos." << endl;
		}
	}
	cout << "Autenticación OK. Bienvenido al sistema de mensajería. ¿Qué acción desea realizar?" << endl;
	cliente.mostrarMenu();
	return NULL;
}

void Cliente::mostrarMenu() {
	do {
		cout << "1) Enviar Mensaje" << endl;
		cout << "2) Recibir Mensajes" << endl;
		cout << "3) Lorem Ipsum" << endl;
		cout << "4) Desconectar" << endl;
		cout << "5) Salir" << endl;
		cout << "Elija la accion que desea realizar: ";
		cin >> opcionMenu;
		} while (opcionMenu < 1 && opcionMenu > 5);
	this -> elegirOpcionDelMenu(opcionMenu);
}

void comenzarEnvio()

void Cliente::elegirOpcionDelMenu(int opcion, list<Cliente> clientesDisponibles){
	int frecuenciaDeEnvios = 0;
	int cantidadMaximaDeEnvios = 0;
	switch (opcion) {
		case 1:
			this -> comenzarEnvio(clientesDisponibles);
			break;
		case 2:
			this->recibir();
			break;
		case 3:
			this->loremIpsum(frecuenciaDeEnvios, cantidadMaximaDeEnvios);
			break;
		case 4:
			this->desconectar();
			break;
		case 5:
			this->salir();
			break;
		default:
			break;
	}
}

list<Cliente> Cliente::conectar(string nombre, string contrasenia) {
	//Se establece la conexion con el servidor mediante autenticacion. El servidor devuelve la lista con todos los usuarios disponibles

	if (connect(socketCliente,(struct sockaddr *)&direccionServidor, sizeof(direccionServidor)) == 0){
	        cout << "ConectandosedireccionServidor.sin_family = AF_INET al puerto: " << this -> puertoServidor << endl;
	        return new list<Cliente>(new Cliente());
	}
	else{
		return NULL;
	}
	//Faltaria que el servidor devuelve la lista con los usuarios disponibles y que confirme la autenticacion del cliente
}

void Cliente::desconectar() {
	//Se cierra la conexion con el servidor
	close(socketCliente);
}

void Cliente::salir() {
	//Se termina la ejecucion del programa
}

void Cliente::enviar(string mensaje, string destinatario) {
	//Se envia un mensaje a un usuario o a todos (este ultimo caso sucede cuando el destinatario es el string "Todos".
	//Hay que realizar el submenu dinamico con todos los usuarios disponibles.
	//Requiere una conexion abierta.
}

list<string> Cliente::recibir() {
	//Se reciben todos los mensajes en la secuencia en la que fueron enviados (creo que habria que usar una cola)
	//Requiere una conexion abierta.
}

void loremIpsum(int frecuenciaDeEnvios, int cantidadMaximaDeEnvios){
	//Toma el texto de un archivo y se envian mensajes en forma ciclica. El tamanio de mensajes y el destinatario son aleatorios
	//Cuando todo el texto fue transmitido, se empieza otra vez desde el inicio
	//Requiere una conexion abierta.
}
