/*
 * Servidor.cpp
 *
 *  Created on: 28 ago. 2016
 *      Author: franco
 */

#include "Servidor.h"
#include <string.h>
#include <stdlib.h>
using namespace std;

Servidor::Servidor(char* nombreArchivoDeUsuarios, int puerto, Logger* logger) {
	this->puerto = puerto;
	this->nombreArchivo = nombreArchivoDeUsuarios;
	this->logger = logger;

	this->welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);
	/*---- Configure settings of the server address struct ----*/
	/* Address family = Internet */
	this->serverAddr.sin_family = AF_INET;
	/* Set port number, using htons function to use proper byte order */
	this->serverAddr.sin_port = htons(puerto);
	/* Set IP address to localhost */
	//this->serverAddr.sin_addr.s_addr = inet_addr("192.168.1.10");
	this->serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	/* Set all bits of the padding field to 0 */
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
	/*---- Bind the address struct to the socket ----*/
	bind(this->welcomeSocket, (struct sockaddr *) &serverAddr,
			sizeof(serverAddr));
	this->datosUsuarios = new list<Datos>();
	this->listaMensajesProcesados = new list<MensajesProcesados>();
	stringstream ss;
	ss << puerto;
	mensaje = "Se creó correctamente el servidor en el puerto: " + ss.str() + ", ip: 192.168.1.10" + "\n";
	this->guardarLog(mensaje, DEBUG);
	this->guardarDatosDeUsuarios();
}

Servidor::~Servidor() {
}

void Servidor::guardarDatosDeUsuarios() {
	string linea, csvItem;
	int nroItem;
	ifstream myfile(this->nombreArchivo);
	if (myfile.is_open()) {
		while (getline(myfile, linea)) {
			Datos datosCapturados;
			MensajesProcesados mensajesProcesados;
			nroItem = 0;
			istringstream lineaActual(linea);
				while (getline(lineaActual, csvItem, ',')) {
				if (nroItem == 0) {
					datosCapturados.nombre = csvItem;
					mensajesProcesados.destinatario = csvItem;
				}
				if (nroItem == 1) {
					datosCapturados.contrasenia = csvItem;
				}
				nroItem++;
			}
			queue<Mensaje>* colaMensajes = new queue<Mensaje>;
			mensajesProcesados.mensajes = colaMensajes;
			datosUsuarios->push_back(datosCapturados);
			listaMensajesProcesados->push_back(mensajesProcesados);
		}
	}
	myfile.close();
	mensaje = "Se leyeron los datos de los usuarios desde el archivo usuarios.csv \n";
	this->guardarLog(mensaje, DEBUG);
}

void Servidor::autenticar(string nombre, string contrasenia,
		list<string>& usuarios) {

	bool autenticacionOK = false;
	this->usuarios.clear();
	for (list<Datos>::iterator datoActual = datosUsuarios->begin();
			datoActual != datosUsuarios->end(); datoActual++) {

		Datos usuario;
		usuario = *datoActual;
		if ((strcmp(usuario.nombre.c_str(), nombre.c_str()) == 0)
				&& (strcmp(usuario.contrasenia.c_str(), contrasenia.c_str())
						== 0)) {
			autenticacionOK = true;
		} else {
			usuarios.push_back(usuario.nombre);
		}
	}
	if (autenticacionOK) {
		cout << "Autenticación OK wachin" << endl;
		mensaje = "Autenticación OK \n";
		this->guardarLog(mensaje, DEBUG);
	} else {
		usuarios.clear();
		cout << "Error de autenticación, no nos hackees wachin" << endl;
		string msj = "Error de autenticación: usuario y/o clave incorrectos \n";
		this->guardarLog(msj, INFO);
	}
}

list<Cliente> Servidor::obtenerClientes() {

}

void Servidor::guardarLog(string mensaje, const int nivelDeLog) {
	this->logger->escribir(mensaje, nivelDeLog);
}

list<Mensaje> Servidor::obtenerMensajes(Cliente cliente) {

}

void Servidor::crearMensaje(Mensaje mensaje) {
	this->colaMensajesNoProcesados.push(mensaje);
//this->procesarMensaje(mensaje); //Por ahora invoco a este metodo aca pero deberia ir en un while(1) dentro del mainServidor, para que la cola se mensajes no procesados se procese infinitamente
}

void Servidor::procesarMensajes() {
	if (!colaMensajesNoProcesados.empty()) {
		pthread_mutex_lock(&mutexColaNoProcesados);
		Mensaje mensajeAProcesar = colaMensajesNoProcesados.front();
		colaMensajesNoProcesados.pop();
		pthread_mutex_unlock(&mutexColaNoProcesados);
		for (list<MensajesProcesados>::iterator usuarioActual =
				listaMensajesProcesados->begin();
				usuarioActual != listaMensajesProcesados->end();
				usuarioActual++) {
			MensajesProcesados listaMensajes;
			listaMensajes = *usuarioActual;
			if (listaMensajes.destinatario
					== mensajeAProcesar.getDestinatario()) {
				pthread_mutex_lock(&mutexListaProcesados);
				listaMensajes.mensajes->push(mensajeAProcesar);
				pthread_mutex_unlock(&mutexListaProcesados);
				this->mensaje = "Procesando mensaje para "
						+ listaMensajes.destinatario + "\n";
				this->guardarLog(mensaje, DEBUG);
				cout << mensaje << endl;
			}
		}
	}
}

void Servidor::comenzarEscucha() {
//Metodo que pone al servidor a escuchar si alguien requiere algo.
	this->escuchando = (listen(this->welcomeSocket, MAX_CANT_CLIENTES) == 0);
	mensaje = "El servidor está escuchando... \n";
	this->guardarLog(mensaje, DEBUG);
	cout << "Escuchando conexiones entrantes.." << endl;
}

int Servidor::aceptarConexion() {
//esta funcion acepta las conexiones para cada cliente que lo solicita si es autenticado y devuelve el socket de dicho cliente.
	string nombre, pass;
	char buffer[BUFFER_MAX_SIZE];
	char datosRecibidos[BUFFER_MAX_SIZE];

	this->addr_size = sizeof serverStorage;
	int socketCliente = accept(welcomeSocket,
			(struct sockaddr *) &this->serverStorage, &this->addr_size);
	recv(socketCliente, datosRecibidos, BUFFER_MAX_SIZE, 0);

	cout << "Datos recibidos: " << datosRecibidos << endl;
	mensaje = "Datos recibidos: " + (string) datosRecibidos + "\n";
	this->guardarLog(mensaje, DEBUG);

	splitDatos(datosRecibidos, &nombre, &pass);
	this->autenticar(nombre, pass, usuarios);
	char* resultadoDeLaAutenticacion;
	this->cantClientesConectados += 1;

	if (usuarios.empty()) {
		strcpy(buffer, "Desconectar");
		mensaje = "Se desconecta al usuario " + nombre
				+ " del servidor porque falló la autenticación... \n";
		this->guardarLog(mensaje, INFO);
		send(socketCliente, buffer, BUFFER_MAX_SIZE, 0);
	} else {
		strcpy(buffer, this->serializarLista(usuarios).c_str());
		string bufferS = buffer;
		mensaje = "Enviándole al cliente " + nombre
				+ " la lista de usuarios disponibles \n";
		this->guardarLog(mensaje, DEBUG);
		mensaje = "La lista de usuarios disponibles es: ";
		for (list<string>::iterator i = usuarios.begin(); i != usuarios.end();
				i++) {
			mensaje += (*i);
			mensaje += " ";
		}
		mensaje += "/n";
		this->guardarLog(mensaje, DEBUG);
		send(socketCliente, buffer, BUFFER_MAX_SIZE, 0);
	}
	return socketCliente;
}

void Servidor::finalizarEscucha() {
//Metodo que finaliza la escucha del servidor.
}

queue<Mensaje> Servidor::getColaMensajesNoProcesados() {
	return this->colaMensajesNoProcesados;
}

void Servidor::splitDatos(char* datos, string* nombre, string* pass) {
	int pos = 0;
	bool nombreCompleto = false;
	bool termino = false;
	while (pos < strlen(datos)) {
		if (datos[pos] != ',' && !nombreCompleto) {
			*nombre = *nombre + datos[pos];

			pos++;
		} else {
			pos++;
			nombreCompleto = true;
			*pass = *pass + datos[pos];
		}
	}
}

void Servidor::recibirMensaje() {
	Mensaje* mensajeARecibir = new Mensaje();
	char datosMensaje[BUFFER_MAX_SIZE];
	cout << "Recibir mensaje" << endl;
	recv(this->socketServer, datosMensaje, strlen(datosMensaje), 0);
	mensajeARecibir->setearDatos(datosMensaje);
	cout << "Remitente: " << mensajeARecibir->getRemitente() << endl;
	cout << "Destinatario: " << mensajeARecibir->getDestinatario() << endl;
	cout << "Mensaje: " << mensajeARecibir->getTexto() << endl;
}

void Servidor::setThreadProceso(pthread_t thrProceso) {
	this->threadProcesador = thrProceso;
}

int Servidor::getCantConexiones() {
	stringstream ss;
	ss << this->cantClientesConectados;
	mensaje = "Cantidad de clientes conectados: " + ss.str() + +"\n";
	this->guardarLog(mensaje, DEBUG);
	return this->cantClientesConectados;
}

string Servidor::serializarLista(list<string> datos) {
	string buffer = "";
	for (list<string>::iterator i = datos.begin(); i != datos.end(); i++) {
		cout << (*i) << endl;
		buffer = buffer + *i + ",";
	}
	return buffer;
}

list<string> Servidor::agregarDestinatarios(string remitente) {
	list<string> destinatarios;
	for (list<Servidor::Datos>::iterator datoActual = datosUsuarios->begin();
			datoActual != datosUsuarios->end(); datoActual++) {
		Datos usuario;
		usuario = *datoActual;
		if (usuario.nombre != remitente) {
			destinatarios.push_back(usuario.nombre);
		}
	}
	return destinatarios;
}
string Servidor::traerMensajesProcesados(char* nombreCliente) {

	queue<Mensaje>* colaDeMensajes;

	for (list<Servidor::MensajesProcesados>::iterator datoActual =
			listaMensajesProcesados->begin();
			datoActual != listaMensajesProcesados->end(); datoActual++) {

		MensajesProcesados mensaje;
		mensaje = *datoActual;
		if (mensaje.destinatario == nombreCliente) {
			colaDeMensajes = mensaje.mensajes;
		}
	}

	string mensajesConcatenados = concatenarMensajes(colaDeMensajes);

	return mensajesConcatenados;

}
string Servidor::concatenarMensajes(queue<Mensaje>* colaDeMensajes) {

	Mensaje mensaje;
	string mensajesConcatenados = "";
	if (colaDeMensajes->empty()) {
		string noHayMensajes = "#noHayMensajes#";
		mensajesConcatenados.append(noHayMensajes);
	}
	while (!colaDeMensajes->empty()) {
		mensaje = colaDeMensajes->front();
		colaDeMensajes->pop();
		mensajesConcatenados.append(mensaje.getRemitente());
		mensajesConcatenados.append("|");
		mensajesConcatenados.append(mensaje.getTexto());
		mensajesConcatenados.append("#");
	}

	return mensajesConcatenados;
}
