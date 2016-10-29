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
	int clienteID;
};

struct parametrosThreadEncolarMensaje {
	Mensaje* mensajeNoProcesado;
	Servidor* servidor;
};

struct parametrosThreadEnviarMensajeProcesado {
	Servidor* servidor;
	string usuario;
	int socketCliente;
};


bool stringTerminaCon(std::string const &fullString,
		std::string const &ending) {
	if (fullString.length() >= ending.length()) {
		return (0
				== fullString.compare(fullString.length() - ending.length(),
						ending.length(), ending));
	} else {
		return false;
	}
}

bool fileExists(string fileName) {
	ifstream infile(fileName.c_str());
	return infile.good();
}

//---------------------THREAD PARA ENCOLAR MENSAJES EN LA COLA PRINCIPAL---------------------------------------------

void* encolar(void* arg) {
	parametrosThreadEncolarMensaje parametrosEncolarMensaje = *(parametrosThreadEncolarMensaje*) arg;
	Servidor* servidor = parametrosEncolarMensaje.servidor;
	Mensaje* mensaje = parametrosEncolarMensaje.mensajeNoProcesado;

	if( mensaje!= NULL ){
		pthread_mutex_lock( &parametrosEncolarMensaje.servidor->mutexColaNoProcesados);
		servidor->crearMensaje(*mensaje);
		pthread_mutex_unlock( &parametrosEncolarMensaje.servidor->mutexColaNoProcesados);
		mensaje->~Mensaje();
	}
	return NULL;
}

void encolarMensaje(string remitente, string destinatario, string mensaje, Servidor* servidor) {
	pthread_t threadEncolarMensaje;
	parametrosThreadEncolarMensaje parametrosEncolarMensaje;
	if(remitente != "" && destinatario != "" && mensaje != ""){
		parametrosEncolarMensaje.mensajeNoProcesado = new Mensaje(remitente,destinatario, mensaje);
		parametrosEncolarMensaje.servidor = servidor;
		pthread_create(&threadEncolarMensaje, NULL, &encolar, &parametrosEncolarMensaje);
		pthread_detach(threadEncolarMensaje); //lo marco
	}
}
//-----------------------------------------------------------------------------------------------------------

//-----------------------SE ENVIAN LOS MENSAJES A LOS CLIENTES CADA CIERTO TIEMPO----------------------------

void* enviarMensajes(void* arg) {
	parametrosThreadEnviarMensajeProcesado* parametros = (parametrosThreadEnviarMensajeProcesado*) arg;
	Servidor* servidor = parametros->servidor;
	string usuario = parametros->usuario;
	int socket = parametros->socketCliente;

	pthread_mutex_lock(&servidor->mutexListaProcesados);
	string mensajesProcesados = servidor->traerMensajesProcesados(usuario);
	pthread_mutex_unlock(&servidor->mutexListaProcesados);
    if( mensajesProcesados != "" ) {
		cout << "mensaje para: "<< usuario <<"   mensaje: " << mensajesProcesados << endl;
    	int largo = strlen(mensajesProcesados.c_str());
		stringstream iss;
		iss << largo;
		string largoString = iss.str();
		char buffer[BUFFER_MAX_SIZE];
		memset(buffer, '\0', BUFFER_MAX_SIZE);
		int inicio = 0;
		int ok;
		if (largo > BUFFER_MAX_SIZE) {
			while (BUFFER_MAX_SIZE < (largo - inicio)) {
				string mensajeSpliteado = mensajesProcesados.substr(inicio, BUFFER_MAX_SIZE);
				strcpy(buffer, mensajeSpliteado.c_str());
				/*do{
					ok = send(socket, buffer, strlen(buffer), 0);
				}while (ok == 0 );
				*/
				//pthread_mutex_lock(&servidor->mutexSocket);
				send(socket, buffer, strlen(buffer), 0);
				//pthread_mutex_unlock(&servidor->mutexSocket);
				inicio += BUFFER_MAX_SIZE;
			}
			string mensajeSpliteado = mensajesProcesados.substr(inicio, largo);
			strcpy(buffer, mensajeSpliteado.c_str());
		} else {
			strcpy(buffer, mensajesProcesados.c_str());
		}
		//pthread_mutex_lock(&servidor->mutexSocket);
		send(socket, buffer, strlen(buffer), 0);
		//pthread_mutex_unlock(&servidor->mutexSocket);
    }
	return NULL;
}

void* cicloEnviarMensajes(void* arg){
	parametrosThreadEnviarMensajeProcesado* parametros = (parametrosThreadEnviarMensajeProcesado*) arg;
	Servidor* servidor =  parametros->servidor;
	usleep(2000000);
	if (servidor->getJugadoresConectados()->size() == servidor->getCantJugadoresConectadosMax()) {
		while (servidor->escuchando) {
			usleep(50000);
			if(parametros->usuario != ""){
				enviarMensajes((void*)parametros);
			}
		}
	}

}
//-----------------------------------------------------------------------------------------------

/*void enviarMensajesProcesadosA(string usuario, Servidor* servidor, int socket) {

	pthread_t threadEnviarMensajesProcesados;
	parametrosThreadEnviarMensajeProcesado parametrosMensajesProcesados;
	if (usuario != "") {
		parametrosMensajesProcesados.usuario = usuario;
		parametrosMensajesProcesados.servidor = servidor;
		parametrosMensajesProcesados.socketCliente = socket;
	}
	int ok = pthread_create(&threadEnviarMensajesProcesados, NULL, &procesar, &parametrosMensajesProcesados);
	pthread_join(threadEnviarMensajesProcesados, NULL);
	//pthread_detach(threadEnviarMensajesProcesados); //lo marco
}*/


void* cicloProcesarMensajes(void* arg) {
	Servidor* servidor = (Servidor*) arg;
	while (servidor->escuchando) {
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
	//servidor->guardarLog("Escuchando al cliente: " + nombre + string(".\n"),INFO);
	char bufferRecibido[BUFFER_MAX_SIZE];
	bool conectado = true;
	servidor->setJugadorConectado(nombre);

	//----------se crea el hilo para enviar mensajes al cliente en cuestion cada cierto tiempo--------
	parametrosThreadEnviarMensajeProcesado parametrosEnviar;
	parametrosEnviar.servidor = servidor;
	parametrosEnviar.socketCliente = socketCliente;
	parametrosEnviar.usuario = nombre;
	pthread_t threadEnviar;
    pthread_create(&threadEnviar, NULL, &cicloEnviarMensajes, &parametrosEnviar);
    //----------------------------------------------------------------------------------------

    while (conectado and servidor->escuchando) {
		//en este loop se van a gestionar los send y receive del cliente. aca se va a distinguir que es lo que quiere hacer y actuar segun lo que quiera el cliente.
		string datosRecibidos;
		//pthread_mutex_lock(&servidor->mutexSocket);
		int largoRequest = recv(socketCliente, bufferRecibido, BUFFER_MAX_SIZE,0); //recibo por primera vez
		//pthread_mutex_unlock(&servidor->mutexSocket);
		if (largoRequest > 0) {
			datosRecibidos.append(bufferRecibido, largoRequest);
			while (largoRequest >= BUFFER_MAX_SIZE and !stringTerminaCon(datosRecibidos, "@")) {
				//mientras haya cosas que leer, sigo recibiendo.
				//pthread_mutex_lock(&servidor->mutexSocket);
				largoRequest = recv(socketCliente, bufferRecibido,BUFFER_MAX_SIZE, 0);
				//pthread_mutex_unlock(&servidor->mutexSocket);
				datosRecibidos.append(bufferRecibido, largoRequest);
			}
			if (largoRequest < 0) {
				string mensaje =
						"ERROR: Ocurrió un problema con el socket del cliente: "
								+ nombre + string(".\n");
				conectado = false;
				servidor->setJugadorDesconectado(nombre);
				servidor->guardarLog(mensaje, DEBUG);
				servidor->restarCantidadClientesConectados();
			}
			else if (largoRequest == 0){
				conectado = false;
				servidor->setJugadorDesconectado(nombre);
				servidor->guardarLog("Se cerró la conexión con el cliente: " + nombre + string(".\n"),DEBUG);
			}
			else {
				//en el formato siempre recibimos primero el metodo que es un entero.
				//1 representa enviar un mensaje, 2 representa recibir mis mensajes, 3 verificar la conexion.
				char* datos = strdup(datosRecibidos.c_str());
				char* metodo = strtok(datos, "|");
				int accion = atoi(metodo); //convierto a entero el metodo recibido por string
				switch (accion) {
					case 1: { //1 es enviar(se envian al servidor los eventos de los clientes)
						char* remitente = strtok(NULL, "|");
						char* destinatario = strtok(NULL, "|");
						char* mensaje = strtok(NULL, "#");
						if( remitente != NULL && destinatario != NULL && mensaje != NULL ){
							encolarMensaje(string(remitente), string(destinatario), string(mensaje), servidor);
						}
						//usleep(50000);
						break;
					}
					/*case 2: { //2 es recibir
						char* usuarioQueSolicita = strtok(NULL, "#");
						//usleep(50000);
						if (usuarioQueSolicita != NULL) {
							enviarMensajesProcesadosA(string(usuarioQueSolicita), servidor, socketCliente);
						}
						break;
					}*/
					case 3:{//3 es verificar conexion
						char buffer[BUFFER_MAX_SIZE] = "Escuchando";
						int ok = send(socketCliente,buffer,strlen(buffer),0);
						break;
					}
					case 4:{//4 es se desconecto el cliente, es un mensaje de log diferente al si se corta la conexion
						servidor->restarCantidadClientesConectados();
						std::ostringstream oss;
						oss << servidor->getCantConexiones();
						string conectados = oss.str();
						pthread_exit(NULL);
						break;
					}
					case 5:{
						char comenzoJuego[BUFFER_MAX_SIZE];
						string comenzo;
						if (servidor->getJugadoresConectados()->size() == servidor->getCantJugadoresConectadosMax()){
							string jugadoresInicio = servidor->getEstadoInicialSerializado();
							comenzo = "0|"  + jugadoresInicio + "@";
							servidor->iniciarCamara();
						}
						else{
							comenzo = "1@";
						}
						strcpy(comenzoJuego,comenzo.c_str());
						send(socketCliente,comenzoJuego,strlen(comenzoJuego),0);
						break;
					}
					case 6:{//6 es enviarHandShake
						char* cliente = strtok(NULL, "|");
						if (cliente != NULL)
						{
							servidor->enviarHandshake(socketCliente,cliente);
							//usleep(1000000);
							//enviarMensajesProcesadosA(cliente, servidor, socketCliente);
						}
						break;
					}
				}
			}
		} else {
			string mensaje ="";
			if (largoRequest == 0){
				mensaje = "Se cerró la conexión con el cliente: " + nombre + string(".\n");
			}
			else{
				mensaje = "ERROR: Ocurrió un problema con el socket del cliente: " + nombre + string(".\n");
			}
			servidor->setJugadorDesconectado(nombre);
			conectado = false;
			servidor->guardarLog(mensaje, DEBUG);
			servidor->restarCantidadClientesConectados();
			std::ostringstream oss;
			oss << servidor->getCantConexiones();
			string conectados = oss.str();
			//servidor->guardarLog("Cantidad de clientes conectados: " + conectados + string("\n"),INFO);
		}
	}
    pthread_detach(threadEnviar);
}

void* cicloEscucharConexionesNuevasThreadProceso(void* arg) {
	Servidor* servidor = (Servidor*) arg;
	pthread_t threadProceso;
	int ok = pthread_create(&threadProceso, NULL, &cicloProcesarMensajes, (void*) servidor);
	if (ok != 0)
	{
		servidor->guardarLog("ERROR: Problema al crear el thread de escucha de conexiones \n",DEBUG);
	}
	else{
		servidor->guardarLog("Thread de escucha de conexiones creado correctamente.",DEBUG);
	}
	servidor->setThreadProceso(threadProceso);

	pthread_t thread_id[MAX_CANT_CLIENTES]; //la cantidad maxima de clientes es 6, voy a crear, como mucho 6 threads para manejar dichas conexiones.
	for (int i = 0; i < MAX_CANT_CLIENTES; i++) {
		thread_id[i] = NULL; //inicializo todos en null
	}
	while (servidor->escuchando) {
		//se va a generar un thread para que cada cliente tenga su comunicacion con el servidor.
		pair<int, string> cliente = servidor->aceptarConexion();
		int socketCliente = cliente.first;
		string nombreCliente = cliente.second;
		if (socketCliente <= 0) {
			cout << "Error al conectar al cliente" << endl;
			//servidor->guardarLog("No se pudo conectar al cliente.\n", INFO);
			//servidor->guardarLog("ERROR: Problema con la conexion del socket.\n", DEBUG);
		} else {
			//servidor->guardarLog("Cliente conectado: " + nombreCliente + string("\n"), INFO);
			std::ostringstream oss;
			oss << servidor->getCantConexiones();
			string conectados = oss.str();
			servidor->guardarLog("Cantidad de clientes conectados: " + conectados + string("\n"),INFO);
			//genero un nuevo thread dinamicamente para este cliente
			if (servidor->getCantConexiones() <= MAX_CANT_CLIENTES && servidor->getCantConexiones() > 0) {
				//si todavia hay lugar en el servidor, creo el thread que va a escuchar los pedidos de este cliente
				parametrosThreadCliente parametrosCliente;
				parametrosCliente.socketCli = socketCliente;
				parametrosCliente.serv = servidor;
				parametrosCliente.nombre = nombreCliente;
				parametrosCliente.clienteID = servidor->getCantConexiones() - 1;
				int ok = pthread_create(&thread_id[parametrosCliente.clienteID], NULL,
						&cicloEscuchaCliente, &parametrosCliente); //optimizar ya que si un cliente se desconecta podria causar un problema
				if (ok != 0)
				{
					servidor->guardarLog("ERROR: No se pudo crear el thread de comunicación con el cliente.\n",DEBUG);
				}
				else{
					servidor->guardarLog("Thread de comunicación con el cliente creado correctamente.\n",DEBUG);
				}
				pthread_detach(thread_id[parametrosCliente.clienteID]); //lo marco como detach
			}
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
		if (cin.good() && (modoLogger == 1 || modoLogger == 2)) {
			modoLoggerValido = true;
		} else {
			cin.clear();
			cin.ignore();
			cout << "Error: la opcion ingresada no es valida" << endl;
		}
	} while (!modoLoggerValido);
	Logger* logger = new Logger(modoLogger);
	stringstream modo;
	modo << modoLogger;
	logger->escribir(
			"El nivel de log seleccionado es: " + modo.str() + string(". \n"),
			INFO);
	do {
		cout << "Ingrese el puerto de escucha de la conexion: " << endl;
		cin >> puerto;
		if (cin.good()) {
			puertoValido = true;
			stringstream puertoStr;
			puertoStr << puerto;
			logger->escribir(
					"Puerto ingresado: " + puertoStr.str() + string(". \n"),
					INFO);
		} else {
			cin.clear();
			cin.ignore();
			cout << "Error: el dato ingresado debe ser un numero" << endl;
			logger->escribir("ERROR: Puerto inválido. \n", INFO);
		}
	} while (!puertoValido);
	bool existeArchivo;
	do {
		cout << "Ingrese el nombre del archivo de usuarios: " << endl;
		cin >> archivoUsers;
		logger->escribir("Archivo ingresado: " + archivoUsers + string(". \n"),
				INFO);
		existeArchivo = fileExists(archivoUsers);
		if (!existeArchivo) {
			logger->escribir("ERROR: El archivo no existe. \n", INFO);
			cout << "Error: El archivo no existe." << endl;
		}
	} while (!existeArchivo);
	logger->escribir("Archivo encontrado.\n", DEBUG);
	char* archivo = strdup(archivoUsers.c_str());
	Servidor* servidor = new Servidor(archivo, puerto, logger);

	do {
		servidor->comenzarEscucha();
	} while (!servidor->escuchando);

	int threadOk = pthread_create(&thrConexiones, NULL,
			&cicloEscucharConexionesNuevasThreadProceso, servidor);
	if (threadOk != 0) {
		cout << "Error inicializando el thread de escucha de conexiones"
				<< endl;
		servidor->guardarLog(
				"ERROR: No se pudo crear el thread de escucha de conexiones.\n",
				DEBUG);
	} else {
		servidor->guardarLog(
				"Creación de thread de escucha de conexiones OK. \n", DEBUG);
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
		if (salir == "q") {
			string mensaje = "Se ha cerrado el servidor";
			servidor->guardarLog(mensaje, INFO);
			servidor->finalizarEscucha();
		}
	}
	return 0;
}
