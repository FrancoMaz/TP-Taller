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
	//this->serverAddr.sin_addr.s_addr = inet_addr("192.168.1.11");
	this->serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	/* Set all bits of the padding field to 0 */
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
	/*---- Bind the address struct to the socket ----*/
	bind(this->welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
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
			nroItem = 0;
			istringstream lineaActual(linea);
				while (getline(lineaActual, csvItem, ',')) {
				if (nroItem == 0) {
					datosCapturados.nombre = csvItem;
				}
				if (nroItem == 1) {
					datosCapturados.contrasenia = csvItem;
				}
				nroItem++;
			}
			datosUsuarios->push_back(datosCapturados);
		}

		myfile.close();
		mensaje = "Se leyeron los datos de los usuarios desde el archivo " + string(this->nombreArchivo) + "\n";
		this->guardarLog(mensaje, DEBUG);
	}

}

void Servidor::autenticar(string nombre, string contrasenia, list<string>& usuarios) {

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
			MensajesProcesados mensajesProcesados;
			mensajesProcesados.jugador = new Jugador(usuario.nombre);
			mensajesProcesados.posiciones = new queue<string>();
			this->listaMensajesProcesados->push_back(mensajesProcesados);
		} else {
			usuarios.push_back(usuario.nombre);
		}
	}
	if (autenticacionOK) {
		cout << "Autenticación OK" << endl;
		mensaje = "Autenticación OK \n";
		this->guardarLog(mensaje, DEBUG);
	} else {
		usuarios.clear();
		cout << "Error de autenticación: usuario y/o clave incorrectos" << endl;
		string msj = "Error de autenticación: usuario y/o clave incorrectos \n";
		this->guardarLog(msj, INFO);
	}
}


void Servidor::guardarLog(string mensaje, const int nivelDeLog) {
	this->logger->escribir(mensaje, nivelDeLog);
}


void Servidor::crearMensaje(Evento mensaje) {
	this->colaMensajesNoProcesados.push(mensaje);
}

void Servidor::procesarMensajes() {

	if (!colaMensajesNoProcesados.empty()) {
		pthread_mutex_lock(&mutexColaNoProcesados);
		Evento mensajeAProcesar = colaMensajesNoProcesados.front();
		colaMensajesNoProcesados.pop();
		pthread_mutex_unlock(&mutexColaNoProcesados);
		for (list<MensajesProcesados>::iterator usuarioActual = listaMensajesProcesados->begin();
				usuarioActual != listaMensajesProcesados->end();usuarioActual++) {
			MensajesProcesados mensaje = *usuarioActual;
			pair<int,int> nuevaPosicion = this->actualizarPosicion(mensajeAProcesar.getTeclaPresionada());
			mensaje.jugador->actualizarPosicion(nuevaPosicion);
			if (mensaje.jugador->getNombre() != mensajeAProcesar.getRemitente()) {
				mensaje.posiciones->push(mensaje.jugador->getStringJugador());
				/*this->mensaje = "Procesando mensaje para " + listaMensajes.destinatario + "\n";
				this->guardarLog(mensaje, DEBUG);*/
			}
		}
	}
}

pair<int,int> Servidor::actualizarPosicion(SDL_Keycode teclaPresionada) {
	pair<int,int> nuevaPosicion;
	if (teclaPresionada == SDLK_RIGHT)
	{
		nuevaPosicion.first += 1;
	}
	if (teclaPresionada == SDLK_LEFT)
	{
		nuevaPosicion.first -= 1;
	}
	if (teclaPresionada == SDLK_UP)
	{
		nuevaPosicion.second -= 1;
	}
	if (teclaPresionada == SDLK_DOWN)
	{
		nuevaPosicion.second += 1;
	}
	return nuevaPosicion;
}

void Servidor::comenzarEscucha() {
//Metodo que pone al servidor a escuchar si alguien requiere algo.
	this->escuchando = (listen(this->welcomeSocket, MAX_CANT_CLIENTES) == 0);
	mensaje = "El servidor está escuchando... \n";
	this->guardarLog(mensaje, DEBUG);
	cout << "Escuchando conexiones entrantes.." << endl;
}

pair<int,string> Servidor::aceptarConexion() {
//esta funcion acepta las conexiones para cada cliente que lo solicita si es autenticado y devuelve el socket de dicho cliente.
	string nombre, pass;
	char buffer[BUFFER_MAX_SIZE];
	char datosRecibidos[BUFFER_MAX_SIZE];
	pair<int,string> cli;

	this->addr_size = sizeof serverStorage;
	int socketCliente = accept(welcomeSocket,(struct sockaddr *) &this->serverStorage, &this->addr_size);
	int ok = recv(socketCliente, datosRecibidos, BUFFER_MAX_SIZE, 0);
	if (ok < 0){
		this->guardarLog("No se pudieron recibir los datos para la conexion.",INFO);
		this->guardarLog("ERROR: Problema con el recv del socket.",DEBUG);
		cli.first = ok;
		cli.second = "";
		return cli;
	}
	cout << "Datos recibidos: " << datosRecibidos << endl;
	mensaje = "Datos recibidos: " + (string) datosRecibidos + string("\n");
	this->guardarLog(mensaje, DEBUG);

	splitDatos(datosRecibidos, &nombre, &pass);
	this->autenticar(nombre, pass, usuarios);
	char* resultadoDeLaAutenticacion;
    int okAutenticar = 1;
	if (usuarios.empty()) {
		okAutenticar = -1;
		strcpy(buffer, "Desconectar");
		mensaje = "Se desconecta al usuario " + nombre + " del servidor porque falló la autenticación... \n";
		//this->cantClientesConectados -= 1;
		this->guardarLog(mensaje, INFO);
		ok = send(socketCliente, buffer, BUFFER_MAX_SIZE, 0);
		cli.first = -1;
		cli.second = "";
		return cli;
	}
	else {
		this->cantClientesConectados += 1;
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
		mensaje += "\n";
		this->guardarLog(mensaje, DEBUG);
		ok = send(socketCliente, buffer, BUFFER_MAX_SIZE, 0);
	}
	if (ok < 0){
		this->guardarLog("No se pudieron enviar los datos de la lista a traves de la conexion del cliente " + nombre + ".",INFO);
		this->guardarLog("ERROR: Problema con el send del socket.",DEBUG);
		cli.first = -1;
		cli.second = "";
		return cli;
	}
	//this->cantClientesConectados += 1;
	cli.first = socketCliente;
	cli.second = nombre;
	return cli;
}

void Servidor::finalizarEscucha() {
	escuchando = false;
	close(welcomeSocket);
}

queue<Evento> Servidor::getColaMensajesNoProcesados() {
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
	return this->cantClientesConectados;
}

string Servidor::serializarLista(list<string> datos) {
	string buffer = "";
	for (list<string>::iterator i = datos.begin(); i != datos.end(); i++) {
		//cout << (*i) << endl;
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

	queue<string>* posiciones;
	for (list<MensajesProcesados>::iterator datoActual =
			listaMensajesProcesados->begin();
			datoActual != listaMensajesProcesados->end(); datoActual++) {
		MensajesProcesados mensaje;
		mensaje = *datoActual;
		if (mensaje.jugador->getNombre() == nombreCliente) {
			posiciones = mensaje.posiciones;
		}
	}
	string mensajesConcatenados = concatenarMensajes(posiciones);
	return mensajesConcatenados;

}
string Servidor::concatenarMensajes(queue<string>* posiciones) {

	string mensajePosiciones;
	string mensajesConcatenados = "";
	if (posiciones->empty()) {
		string noHayMensajes = "#noHayMensajes#";
		mensajesConcatenados.append(noHayMensajes);
	}
	while (!posiciones->empty()) {
		mensajePosiciones = posiciones->front();
		posiciones->pop();
		mensajesConcatenados += mensajePosiciones;
	}
	string lala = mensajesConcatenados.substr(0, mensajesConcatenados.length() -1);
	lala += "@";

	return lala;
}

void Servidor::restarCantidadClientesConectados(){
	this->cantClientesConectados -= 1;
}
