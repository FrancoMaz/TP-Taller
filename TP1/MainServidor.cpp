/*
 * mainServidor.cpp
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
#include "Servidor.h"
#include <pthread.h>

using namespace std;

struct parametrosThreadCliente {
	//estructura que sirve para guardar los parametros que se le pasan a la funcion del thread.
	Servidor* serv;
	int socketCli;
	string nombre;
};

struct parametrosThreadEncolarMensaje {
	Mensaje* mensajeNoProcesado;
	Servidor* servidor;
};


bool stringTerminaCon(std::string const &fullString,
		std::string const &ending) {
	if (fullString.length() >= ending.length()) {
		return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
	} else {
		return false;
	}
}

bool fileExists(string fileName) {
	ifstream infile(fileName.c_str());
	return infile.good();
}

void* encolar(void* arg) {
	parametrosThreadEncolarMensaje parametrosEncolarMensaje =
			*(parametrosThreadEncolarMensaje*) arg;
	Servidor* servidor = parametrosEncolarMensaje.servidor;
	Mensaje* mensaje = parametrosEncolarMensaje.mensajeNoProcesado;
	servidor->mensaje = "Encolando mensaje: " + mensaje->getTexto() + ". De: " + mensaje->getRemitente() + ". Para: " + mensaje->getDestinatario() + ". \n";
	servidor->guardarLog(servidor->mensaje, DEBUG);
	cout << servidor->mensaje << endl;
	if (mensaje->getDestinatario().compare("Todos") != 0) {
		pthread_mutex_lock(&parametrosEncolarMensaje.servidor->mutexColaNoProcesados);
		servidor->crearMensaje(*mensaje);
		servidor->mensaje = "Encolando mensaje: " + mensaje->getTexto() + ". De: " + mensaje->getRemitente() + ". Para: " + mensaje->getDestinatario() + ". \n";
		servidor->guardarLog(servidor->mensaje, DEBUG);
		pthread_mutex_unlock(&parametrosEncolarMensaje.servidor->mutexColaNoProcesados);
	}
	else {
		list<string> destinatarios = servidor->agregarDestinatarios(mensaje->getRemitente());
		for (list<string>::iterator datoActual = destinatarios.begin(); datoActual != destinatarios.end(); datoActual++) {
			string usuario;
			usuario = *datoActual;
			if (usuario != mensaje->getRemitente()){
				Mensaje* msj = new Mensaje(mensaje->getRemitente(),usuario,mensaje->getTexto());
				servidor->mensaje = "Encolando mensaje: " + msj->getTexto() + ". De: " + msj->getRemitente() + ". Para: " + msj->getDestinatario() + ". \n";
				pthread_mutex_lock(&parametrosEncolarMensaje.servidor->mutexColaNoProcesados);
				parametrosEncolarMensaje.servidor->crearMensaje(*msj);
				pthread_mutex_unlock(&parametrosEncolarMensaje.servidor->mutexColaNoProcesados);
				servidor->guardarLog(servidor->mensaje, DEBUG);
			}
		}
	}
	return NULL;
}

void encolarMensaje(char* remitente, char* destinatario, char* mensaje,
		Servidor* servidor) {
	pthread_t threadEncolarMensaje;
	parametrosThreadEncolarMensaje parametrosEncolarMensaje;
	parametrosEncolarMensaje.mensajeNoProcesado = new Mensaje(remitente,
			destinatario, mensaje);
	parametrosEncolarMensaje.servidor = servidor;
	pthread_create(&threadEncolarMensaje, NULL, &encolar,
			&parametrosEncolarMensaje);
	pthread_detach(threadEncolarMensaje); //lo marco
}

void* cicloProcesarMensajes(void* arg)
{
	Servidor* servidor = (Servidor*)arg;
	while(1)
	{
		servidor->procesarMensajes();
	}
}

void* cicloEscuchaCliente(void* arg) {
	//esta funcion es la que se va a encargar de hacer send y recv de los enviar/recibir/desconectar
	//es decir, esta funcion es la que va a estar constantemente haciendo send y recv del socket del cliente y detectando lo que quiere hacer.
	struct parametrosThreadCliente* parametros = (parametrosThreadCliente*) arg;
	Servidor* servidor = parametros->serv;
	string nombre = parametros->nombre;
	int socketCliente = parametros->socketCli;
	char bufferRecibido[BUFFER_MAX_SIZE];
	bool conectado = true;
	while (conectado) {
		//en este loop se van a gestionar los send y receive del cliente. aca se va a distinguir que es lo que quiere hacer y actuar segun lo que quiera el cliente.
		string datosRecibidos;
		int largoRequest = recv(socketCliente, bufferRecibido, BUFFER_MAX_SIZE,0); //recibo por primera vez
		if (largoRequest > 0) {
			datosRecibidos.append(bufferRecibido, largoRequest);
			cout << largoRequest << endl;
			while (largoRequest >= BUFFER_MAX_SIZE
					and !stringTerminaCon(datosRecibidos, "#")) {
				//mientras haya cosas que leer, sigo recibiendo.
				largoRequest = recv(socketCliente, bufferRecibido,
						BUFFER_MAX_SIZE, 0);
				cout << largoRequest << endl;
				datosRecibidos.append(bufferRecibido, largoRequest);
			}
			//en el formato siempre recibimos primero el metodo que es un entero.
			//1 representa enviar un mensaje, 2 representa recibir mis mensajes, 3 desconectar.
			char* datos = strdup(datosRecibidos.c_str());
			char* metodo = strtok(datos, "|");
			int accion = atoi(metodo); //convierto a entero el metodo recibido por string
			switch (accion) {
			case 1: { //1 es enviar
				char* remitente = strtok(NULL,"|");
				char* destinatario = strtok(NULL,"|");
				char* mensaje = strtok(NULL,"#");
				encolarMensaje(remitente, destinatario, mensaje,servidor);
				break;
			}
			case 2: { //2 es recibir
				char* usuarioQueSolicita = strtok(NULL, "#");
				string mensajesProcesados = servidor->traerMensajesProcesados(usuarioQueSolicita);
				char buffer[1024];
				strcpy(buffer,mensajesProcesados.c_str());//aca muere, el problema es este strcpy y el string y char*

				int largo = strlen(mensajesProcesados.c_str());
				largo -= send(socketCliente, buffer, largo +1, 0);
					while (largo > 0)
					{
						largo -= send(socketCliente, buffer, largo + 1, 0);
					}
				break;
			}
			case 3: { //3 es desconectar
				break;
			}

			}
		} else {
			if (largoRequest == 0)
			{

			}
			else if (largoRequest == -1)
			{
				string mensaje = "Ocurrió un problema en el socket para el cliente: " + nombre + ". \n";
				servidor->guardarLog(mensaje, DEBUG);
			}
			conectado = false;
			string mensaje = "Se cerró la conexión del servidor por problemas de red. \n";
			servidor->guardarLog(mensaje, DEBUG);
		}
	}

}

void* cicloEscucharConexionesNuevasThreadProceso(void* arg) {
	Servidor* servidor = (Servidor*)arg;

	pthread_t threadProceso;
	pthread_create(&threadProceso,NULL,&cicloProcesarMensajes,(void*)servidor);
	servidor->setThreadProceso(threadProceso);

	pthread_t thread_id[MAX_CANT_CLIENTES]; //la cantidad maxima de clientes es 6, voy a crear, como mucho 6 threads para manejar dichas conexiones.
	for (int i = 0; i < MAX_CANT_CLIENTES; i++) {
		thread_id[i] = NULL; //inicializo todos en null
	}
	while (servidor->escuchando) {
		//se va a generar un thread para que cada cliente tenga su comunicacion con el servidor.
		int socketCliente = servidor->aceptarConexion();
		//genero un nuevo thread dinamicamente para este cliente
		if (servidor->getCantConexiones() <= MAX_CANT_CLIENTES) {
			//si todavia hay lugar en el servidor, creo el thread que va a escuchar los pedidos de este cliente
			parametrosThreadCliente parametrosCliente;
			parametrosCliente.socketCli = socketCliente;
			parametrosCliente.serv = servidor;

			pthread_create(&thread_id[servidor->getCantConexiones()], NULL,
					&cicloEscuchaCliente, &parametrosCliente); //optimizar ya que si un cliente se desconecta podria causar un problema
			pthread_detach(thread_id[servidor->getCantConexiones()]); //lo marco como detach
		}
	}
}

Servidor* inicializarServidor() {
	pthread_t thrConexiones;
	int puerto;
	string archivoUsers;
	int modoLogger;
	bool puertoValido = false;
	bool modoLoggerValido = false;
	do {
		cout << "Elija el modo en el que quiere loggear los eventos: " << endl;
		cout << "1) INFO" << endl;
		cout << "2) DEBUG" << endl;
		cin >> modoLogger;
		if (cin.good() && (modoLogger == 1 || modoLogger == 2))
		{
			modoLoggerValido = true;
		}
		else
		{
			cin.clear();
			cin.ignore();
			cout << "Error: la opcion ingresada no es valida" << endl;
		}
	} while (!modoLoggerValido);
	Logger* logger = new Logger(modoLogger);
	logger->escribir("El nivel de log seleccionado es :" + modoLogger + string(". \n"),INFO);
	do {
		cout << "Ingrese el puerto de escucha de la conexion: " << endl;
		cin >> puerto;
		if (cin.good())
		{
			puertoValido = true;
			logger->escribir("Puerto ingresado: " + puerto + string("\n"),INFO);
		}
		else
		{
			cin.clear();
			cin.ignore();
			cout << "Error: el dato ingresado debe ser un numero" << endl;
			logger->escribir("ERROR: Puerto inválido. \n",INFO);
		}
	} while (!puertoValido);
	bool existeArchivo;
	do{
		cout << "Ingrese el nombre del archivo de usuarios: " << endl;
		cin >> archivoUsers;
		logger->escribir("Archivo ingresado: " + archivoUsers + string(". \n"),INFO);
		existeArchivo = fileExists(archivoUsers);
		if (!existeArchivo){
			logger->escribir("ERROR: El archivo no existe. \n",INFO);
			cout << "Error: El archivo no existe." << endl;
		}
	} while (!existeArchivo);
	logger->escribir("Archivo encontrado.\n",DEBUG);
	char* archivo = strdup(archivoUsers.c_str());
	Servidor* servidor = new Servidor(archivo, puerto, logger);

	do {
		//aca deberia verificar los datos del nombre de archivo usuarios y del puerto,
		//para ver si mato al servidor o no.
		servidor->comenzarEscucha();
	} while (!servidor->escuchando);

	int threadOk = pthread_create(&thrConexiones, NULL,
		&cicloEscucharConexionesNuevasThreadProceso, servidor);
	if (threadOk != 0){
		cout << "Error inicializando el thread de proceso" << endl;
	}
	pthread_detach(thrConexiones);
	return servidor;
}

int main() {
	Servidor* servidor = inicializarServidor();
	cout << "Escriba q y presione enter para salir.. \n";
	string salir = "";
	while (servidor->escuchando) {
		cin >> salir;
		if (salir == "q"){
			servidor->escuchando = false;
		}
	}
	return 0;
}
