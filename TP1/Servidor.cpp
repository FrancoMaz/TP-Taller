/*
 * Servidor.cpp
 *
 *  Created on: 28 ago. 2016
 *      Author: franco
 */

#include "Servidor.h"
#include <string.h>
using namespace std;

Servidor::Servidor(char* nombreArchivoDeUsuarios, int puerto) {
	this->puerto = puerto;
	this->nombreArchivo = nombreArchivoDeUsuarios;
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
	}
	myfile.close();
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
				&& (strcmp(usuario.contrasenia.c_str(), contrasenia.c_str())== 0)) {
			autenticacionOK = true;
		} else {
			usuarios.push_back(usuario.nombre);
		}
	}

	if (autenticacionOK) {
		cout << "Autenticación OK wachin" << endl;
	} else {
		usuarios.clear();
		cout << "Error de autenticación, no nos hackees wachin" << endl;
	}
}

list<Cliente> Servidor::obtenerClientes() {

}

void Servidor::guardarLog() {
//Guarda toda la actividad en un archivo de texto

}

list<Mensaje> Servidor::obtenerMensajes(Cliente cliente) {

}

void Servidor::crearMensaje(Mensaje mensaje) {
	this->colaMensajesNoProcesados.push(mensaje);
}

void Servidor::comenzarEscucha() {
	//Metodo que pone al servidor a escuchar si alguien requiere algo.
	this->escuchando = (listen(this->welcomeSocket, MAX_CANT_CLIENTES) == 0);
}


int Servidor::aceptarConexion() {
	//esta funcion acepta las conexiones para cada cliente que lo solicita si es autenticado y devuelve el socket de dicho cliente.
	string nombre, pass;
	char buffer[1024];
	char datosRecibidos[1024];

	this->addr_size = sizeof serverStorage;
	cout << "Escuchando conexiones entrantes.." << endl;

	int socketCliente = accept(welcomeSocket, (struct sockaddr *) &this->serverStorage, &this->addr_size);
	recv(socketCliente, datosRecibidos, 1024, 0);

	cout << "Datos recibidos: " << datosRecibidos << endl;
	splitDatos(datosRecibidos, &nombre, &pass);
	cout << "splitea bien los datos" << endl;
	this->autenticar(nombre, pass, usuarios);
	cout << "sale bien del autenticar" << endl;
	char* resultadoDeLaAutenticacion;
	this->cantClientesConectados += 1;

	if (usuarios.empty()) {
		resultadoDeLaAutenticacion = "No se pudo autenticar";
		cout << resultadoDeLaAutenticacion << endl;
		strcpy(buffer, "Desconectar");
		send(socketCliente, buffer, 1024, 0);
		// si no se puede autenticar debe desconectarse al usuario
	} else {
		resultadoDeLaAutenticacion = "Autenticacion OK";
		cout << resultadoDeLaAutenticacion << endl;
		strcpy(buffer, this->serializarLista(usuarios).c_str());
		string bufferS = buffer;
		cout << bufferS << endl;
		send(socketCliente, buffer, 1024, 0);
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
