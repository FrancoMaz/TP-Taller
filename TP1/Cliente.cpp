#include <stdlib.h>
#include "Cliente.h"
#include <iostream>
#include <string.h>

using namespace std;

Cliente::Cliente(char* ip, int puerto) {
//El cliente se crea con la direccion IP y el puerto en el cual se encuentra disponible el servidor
	this->direccionIP = ip;
	this->puertoServidor = puerto;
	this -> socketCliente = socket(AF_INET, SOCK_STREAM, 0);
	direccionServidor.sin_family = AF_INET;
	direccionServidor.sin_port = htons(this -> puertoServidor);
	direccionServidor.sin_addr.s_addr = inet_addr((const char*) ip);
	memset(direccionServidor.sin_zero, '\0', sizeof direccionServidor.sin_zero);

	this->mostrarMenu(); //Muestra el menu con todas las acciones que puede realizar el cliente
}

Cliente::~Cliente() {
	// TODO Auto-generated destructor stub
}

/*int Cliente::inicializarThreadConexion()
{
	return pthread_create(&(this->threadComunicacion), NULL, &(cicloConexion),&this );
}

void* Cliente::cicloConexion(void* arg)
{
	Cliente cliente = *(Cliente*)arg;
	while(cliente.getOpcionMenu() != 5 and cliente.getOpcionMenu() != 4) //mientras la opcion del menu no sea salir o desconectar..
	{
		cliente.mostrarMenu();
	}
	return NULL;
}*/

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


void Cliente::elegirOpcionDelMenu(int opcion){
	int frecuenciaDeEnvios = 0;
	int cantidadMaximaDeEnvios = 0;
	switch (opcion) {
		case 1:
		{
			string destinatario;
			string mensajeAEnviar;
			cout << "Escriba el nombre del destinatario del mensaje: " << endl;
			cin >> destinatario;
			cout << "Escriba su mensaje: " << endl;
			cin >> mensajeAEnviar;
			this -> enviar(mensajeAEnviar,destinatario);
			break;
		}
		case 2:
		{
			this->recibir();
			break;
		}
		case 3:
		{
			int frecuenciaDeEnvios;
			int cantidadMaximaDeEnvios;
			cout << "Escriba la frecuencia de envios: " << endl;
			cin >> frecuenciaDeEnvios;
			cout << "Escriba la cantidad maxima de envios: " << endl;
			cin >> cantidadMaximaDeEnvios;
			this->loremIpsum(frecuenciaDeEnvios, cantidadMaximaDeEnvios);
			break;
		}
		case 4:
		{
			this->desconectar();
			break;
		}
		case 5:
		{
			this->salir();
			break;
		}
		default:
			break;
	}
}

list<Cliente> Cliente::conectar(string nombre, string contrasenia) {
	//Se establece la conexion con el servidor mediante autenticacion. El servidor devuelve la lista con todos los usuarios disponibles

	list<Cliente> clientesDisponibles;
	if (connect(socketCliente,(struct sockaddr *)&direccionServidor, sizeof(direccionServidor)) == 0){
	    cout << "ConectandosedireccionServidor.sin_family = AF_INET al puerto: " << this -> puertoServidor << endl;
	}
	else{
		cout << "Error conectandose al puerto" << endl;
	}
	return clientesDisponibles;
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
	Mensaje *mensajeAEnviar = new Mensaje(this->nombre, destinatario, mensaje);

}

list<string> Cliente::recibir() {
	//Se reciben todos los mensajes en la secuencia en la que fueron enviados (creo que habria que usar una cola)
	//Requiere una conexion abierta.
}

void Cliente::loremIpsum(int frecuenciaDeEnvios, int cantidadMaximaDeEnvios){
	//Toma el texto de un archivo y se envian mensajes en forma ciclica. El tamanio de mensajes y el destinatario son aleatorios
	//Cuando todo el texto fue transmitido, se empieza otra vez desde el inicio
	//Requiere una conexion abierta.
	for (int i = 0; i < cantidadMaximaDeEnvios; i++)
		{
			int tamanioMensaje;
			int numeroDeClienteAEnviar;
			char cadena[tamanioMensaje];
			string clienteAleatorioAEnviar;
			srand(time(NULL));
			tamanioMensaje = rand();
			numeroDeClienteAEnviar = rand() % this->clientesDisponibles.size();
			ifstream archivo("LoremIpsum.txt");
			archivo.getline(cadena,tamanioMensaje);
			list<Cliente>::iterator iterador = this->clientesDisponibles.begin();
			advance(iterador,numeroDeClienteAEnviar);
			/*clienteAleatorioAEnviar = *iterador->getNombre();
			this -> enviar(cadena,clienteAleatorioAEnviar);*/
		}

}

string Cliente::getNombre() {
	return this->nombre;
}

int Cliente::getOpcionMenu(){
	return this -> opcionMenu;
}

list<Cliente> Cliente::getClientesDisponibles(){
	return this->clientesDisponibles;

}
