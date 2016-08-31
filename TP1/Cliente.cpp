#include <stdlib.h>
#include "Cliente.h"
#include <iostream>
#include <string.h>

using namespace std;

Cliente::Cliente(string ip, int puerto) {
//El cliente se crea con la direccion IP y el puerto en el cual se encuentra disponible el servidor
	this->direccionIP = ip;
	this->puertoServidor = puerto;
	this->mostrarMenu(); //Muestra el menu con todas las acciones que puede realizar el cliente
	this -> socketCliente = socket(AF_INET, SOCK_STREAM, 0);
	direccionServidor.sin_family = AF_INET;
	direccionServidor.sin_port = htons(this -> puertoServidor);
}

Cliente::~Cliente() {
	// TODO Auto-generated destructor stub
}

void Cliente::mostrarMenu() {
	do {
		cout << "1) Conectar" << endl;
		cout << "2) Desconectar" << endl;
		cout << "3) Salir" << endl;
		cout << "4) Enviar" << endl;
		cout << "5) Recibir" << endl;
		cout << "6) Lorem Ipsum" << endl;
		cout << "Elija la accion que desea realizar: ";
		cin >> opcionMenu;
		} while (opcionMenu < 1 && opcionMenu > 6);
	this -> elegirOpcionDelMenu(opcionMenu);
}

void Cliente::elegirOpcionDelMenu(int opcion){
	string nombre = "";
	string contrasenia = "";
	string mensaje = "";
	string destinatario = "";
	int frecuenciaDeEnvios = 0;
	int cantidadMaximaDeEnvios = 0;
	switch (opcion) {
		case 1:
			this -> conectar(nombre, contrasenia);
			break;
		case 2:
			this->desconectar();
			break;
		case 3:
			this->salir();
			break;
		case 4:
			this->enviar(mensaje, destinatario);
			break;
		case 5:
			this->recibir();
			break;
		case 6:
			this->loremIpsum(frecuenciaDeEnvios, cantidadMaximaDeEnvios);
			break;
		default:
			break;
	}
}

list<Cliente> Cliente::conectar(string nombre, string contrasenia) {
	//Se establece la conexion con el servidor mediante autenticacion. El servidor devuelve la lista con todos los usuarios disponibles
	cout << "Ingrese nombre de usuario: ";
	cin >> nombre;
	cout << "Ingrese contrasenia: ";
	cin >> contrasenia;
	if (connect(socketCliente,(struct sockaddr *)&direccionServidor, sizeof(direccionServidor)) == 0)
	        cout << "Conectandose al puerto: " << this -> puertoServidor << endl;
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
