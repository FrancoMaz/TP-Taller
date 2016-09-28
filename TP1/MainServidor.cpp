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
	char* usuario;
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

void* encolar(void* arg) {
	parametrosThreadEncolarMensaje parametrosEncolarMensaje =
			*(parametrosThreadEncolarMensaje*) arg;
	Servidor* servidor = parametrosEncolarMensaje.servidor;
	Mensaje* mensaje = parametrosEncolarMensaje.mensajeNoProcesado;

	/*servidor->mensaje = "Encolando mensaje: " + mensaje->getTexto() + ". De: "
	 + mensaje->getRemitente() + ". Para: " + mensaje->getDestinatario()
	 + ". \n";
	 servidor->guardarLog(servidor->mensaje, DEBUG);*/

	if (mensaje->getDestinatario().compare("Todos") != 0) {
		pthread_mutex_lock(
				&parametrosEncolarMensaje.servidor->mutexColaNoProcesados);
		servidor->crearMensaje(*mensaje);
		servidor->mensaje = "Encolando mensaje: " + mensaje->getTexto()
				+ ". De: " + mensaje->getRemitente() + ". Para: "
				+ mensaje->getDestinatario() + ". \n";
		servidor->guardarLog(servidor->mensaje, DEBUG);
		pthread_mutex_unlock(
				&parametrosEncolarMensaje.servidor->mutexColaNoProcesados);
	} else {
		//genera la lista de destinatarios del remitente en cuestion
		list<string> destinatarios = servidor->agregarDestinatarios(
				mensaje->getRemitente());
		pthread_mutex_lock(
				&parametrosEncolarMensaje.servidor->mutexColaNoProcesados);
		for (list<string>::iterator datoActual = destinatarios.begin();
				datoActual != destinatarios.end(); datoActual++) {
			string usuario;
			usuario = *datoActual;

			if (usuario != mensaje->getRemitente()) {

				Mensaje* msj = new Mensaje(mensaje->getRemitente(), usuario,
						mensaje->getTexto());
				servidor->mensaje = "Encolando mensaje: " + msj->getTexto()
						+ ". De: " + msj->getRemitente() + ". Para: "
						+ msj->getDestinatario() + ". \n";
				parametrosEncolarMensaje.servidor->crearMensaje(*msj);
				servidor->guardarLog(servidor->mensaje, DEBUG);
				msj->~Mensaje();

			}
		}
		pthread_mutex_unlock(
				&parametrosEncolarMensaje.servidor->mutexColaNoProcesados);
		mensaje->~Mensaje();
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
	int ok = pthread_create(&threadEncolarMensaje, NULL, &encolar,
			&parametrosEncolarMensaje);
	if (ok != 0)
	{
		servidor->guardarLog("ERROR: No se pudo crear el thread de encolar mensaje.\n", DEBUG);
	}
	else{
		servidor->guardarLog("Thread de encolar mensaje creado correctamente.\n",DEBUG);
	}
	pthread_detach(threadEncolarMensaje); //lo marco
}

void* procesar(void* arg) {
	parametrosThreadEnviarMensajeProcesado* parametros =
			(parametrosThreadEnviarMensajeProcesado*) arg;
	Servidor* servidor = parametros->servidor;
	char* usuario = parametros->usuario;
	int socket = parametros->socketCliente;

	pthread_mutex_lock(&servidor->mutexListaProcesados);
	string mensajesProcesados = servidor->traerMensajesProcesados(usuario);
	pthread_mutex_unlock(&servidor->mutexListaProcesados);

	int largo = strlen(mensajesProcesados.c_str());
	std::ostringstream oss;
	oss << largo;
	string largoString = oss.str();
	servidor->guardarLog("Tamaño del response: " + largoString + string(".\n"),DEBUG);
	char buffer[BUFFER_MAX_SIZE];
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
			ok = send(socket, buffer, strlen(buffer), 0);
			if (ok == 0){
				servidor->guardarLog("Se cerró la conexión con el cliente " + string(usuario) + string(".\n"),DEBUG);
			}
			else if (ok < 0){
				servidor->guardarLog("ERROR: Ocurrió un problema con el socket del cliente: " + string(usuario)+ string(".\n"),DEBUG);
			}
			inicio += BUFFER_MAX_SIZE;
		}
		string mensajeSpliteado = mensajesProcesados.substr(inicio, largo);
		strcpy(buffer, mensajeSpliteado.c_str());
	} else {
		strcpy(buffer, mensajesProcesados.c_str());
	}
	/*do{
		ok = send(socket, buffer, strlen(buffer), 0);
	}while (ok == 0);
	*/
	ok = send(socket,buffer,strlen(buffer),0);
	if (ok == 0){
		servidor->guardarLog("Se cerró la conexión con el cliente " + string(usuario) + string(".\n"),DEBUG);
	}
	else if (ok < 0){
		servidor->guardarLog("ERROR: Ocurrió un problema con el socket del cliente: " + string(usuario)+ string(".\n"),DEBUG);
	}
	//strcpy(buffer, mensajesProcesados.c_str()); //aca muere, el problema es este strcpy y el string y char*
	servidor->guardarLog("Mensajes para el cliente:" + string(usuario) + ", Mensajes: " + mensajesProcesados + string(".\n"),INFO);

	//cout<<"socket Cliente: "<<socket<<endl;
	//largo -= send(socket, buffer, BUFFER_MAX_SIZE + 1, 0);
	/*while (largo > 0) {
		largo -= send(socket, buffer, BUFFER_MAX_SIZE + 1, 0);
	}*/
	return NULL;
}
void enviarMensajesProcesadosA(char* usuario, Servidor* servidor, int socket) {

	pthread_t threadEnviarMensajesProcesados;
	parametrosThreadEnviarMensajeProcesado parametrosMensajesProcesados;
	parametrosMensajesProcesados.usuario = usuario;
	parametrosMensajesProcesados.servidor = servidor;
	parametrosMensajesProcesados.socketCliente = socket;
	int ok = pthread_create(&threadEnviarMensajesProcesados, NULL, &procesar,
			&parametrosMensajesProcesados);
	if (ok != 0)
	{
		servidor->guardarLog("ERROR: No se pudo crear el thread de recibir mensajes.\n", DEBUG);
	}
	else{
		servidor->guardarLog("Thread de recibir mensajes creado correctamente.\n",DEBUG);
	}
	pthread_join(threadEnviarMensajesProcesados, NULL);
	servidor->guardarLog("Fin envio de mensajes a: " + string(usuario) + string(".\n"),INFO);
	//pthread_detach(threadEnviarMensajesProcesados); //lo marco
}

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
	servidor->guardarLog("Escuchando al cliente: " + nombre + string(".\n"),
			INFO);
	char bufferRecibido[BUFFER_MAX_SIZE];
	bool conectado = true;
	while (conectado and servidor->escuchando) {
		//en este loop se van a gestionar los send y receive del cliente. aca se va a distinguir que es lo que quiere hacer y actuar segun lo que quiera el cliente.
		string datosRecibidos;

		int largoRequest = recv(socketCliente, bufferRecibido, BUFFER_MAX_SIZE,
				0); //recibo por primera vez
		if (largoRequest > 0) {
			datosRecibidos.append(bufferRecibido, largoRequest);
			while (largoRequest >= BUFFER_MAX_SIZE and !stringTerminaCon(datosRecibidos, "@")) {
				//mientras haya cosas que leer, sigo recibiendo.
				largoRequest = recv(socketCliente, bufferRecibido,BUFFER_MAX_SIZE, 0);
				datosRecibidos.append(bufferRecibido, largoRequest);
			}
			if (largoRequest < 0) {
				string mensaje =
						"ERROR: Ocurrió un problema con el socket del cliente: "
								+ nombre + string(".\n");
				conectado = false;
				servidor->guardarLog(mensaje, DEBUG);
				servidor->restarCantidadClientesConectados();
			}
			else if (largoRequest == 0){
				conectado = false;
				servidor->guardarLog("Se cerró la conexión con el cliente: " + nombre + string(".\n"),DEBUG);
			}
			else {
				//en el formato siempre recibimos primero el metodo que es un entero.
				//1 representa enviar un mensaje, 2 representa recibir mis mensajes, 3 verificar la conexion.
				char* datos = strdup(datosRecibidos.c_str());
				char* metodo = strtok(datos, "|");
				int accion = atoi(metodo); //convierto a entero el metodo recibido por string
				switch (accion) {
					case 1: { //1 es enviar
						char* remitente = strtok(NULL, "|");
						char* destinatario = strtok(NULL, "|");
						char* mensaje = strtok(NULL, "#");
						servidor->guardarLog("Request: Enviar Mensaje, Remitente: " + string(remitente)
								 	 	 	 	 + ", Destinatario: " + string(destinatario)
												 	 + ", Mensaje: " + string(mensaje) + string(".\n"),INFO);
						encolarMensaje(remitente, destinatario, mensaje, servidor);
						break;
					}
					case 2: { //2 es recibir
						servidor->guardarLog("Request: Recibir Mensajes. " + nombre + string(".\n"),INFO);
						char* usuarioQueSolicita = strtok(NULL, "#");
						enviarMensajesProcesadosA(usuarioQueSolicita, servidor,
								socketCliente);
						break;
					}
					case 3:{//3 es verificar conexion
						char buffer[BUFFER_MAX_SIZE] = "Escuchando";
						int ok = send(socketCliente,buffer,strlen(buffer),0);
						if (ok > 0){
							servidor->guardarLog("El cliente " + nombre + " sigue conectado.\n", DEBUG);
						}
						else if (ok == 0){
							servidor->guardarLog("Se cerró la conexión con el cliente " + nombre + string(".\n"),DEBUG);
						}
						else{
							servidor->guardarLog("ERROR: Ocurrió un problema con el socket del cliente: " + nombre + string(".\n"),DEBUG);
						}
						break;
					}
					case 4:{//4 es se desconecto el cliente, es un mensaje de log diferente al si se corta la conexion
						servidor->restarCantidadClientesConectados();
						servidor->guardarLog("El cliente " + nombre + " cerró la conexión.\n", INFO);
						std::ostringstream oss;
						oss << servidor->getCantConexiones();
						string conectados = oss.str();
						servidor->guardarLog("Cantidad de clientes conectados: " + conectados + string("\n"),INFO);
						pthread_exit(NULL);
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
			}conectado = false;
			servidor->guardarLog(mensaje, DEBUG);
			servidor->restarCantidadClientesConectados();
			std::ostringstream oss;
			oss << servidor->getCantConexiones();
			string conectados = oss.str();
			servidor->guardarLog("Cantidad de clientes conectados: " + conectados + string("\n"),INFO);
		}
	}
}

void* cicloEscucharConexionesNuevasThreadProceso(void* arg) {
	Servidor* servidor = (Servidor*) arg;
	pthread_t threadProceso;
	int ok = pthread_create(&threadProceso, NULL, &cicloProcesarMensajes,
			(void*) servidor);
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
		if (socketCliente < 0) {
			cout << "Error al conectar al cliente" << endl;
			servidor->guardarLog("No se pudo conectar al cliente.\n", INFO);
			servidor->guardarLog(
					"ERROR: Problema con la conexion del socket.\n", DEBUG);
		} else {
			servidor->guardarLog("Cliente conectado: " + nombreCliente + string("\n"), INFO);
			std::ostringstream oss;
			oss << servidor->getCantConexiones();
			string conectados = oss.str();
			servidor->guardarLog("Cantidad de clientes conectados: " + conectados + string("\n"),INFO);
			//genero un nuevo thread dinamicamente para este cliente
			if (servidor->getCantConexiones() <= MAX_CANT_CLIENTES
					&& servidor->getCantConexiones() > 0) {
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
