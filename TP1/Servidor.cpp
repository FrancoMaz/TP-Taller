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
	this->serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	/* Set all bits of the padding field to 0 */
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
	/*---- Bind the address struct to the socket ----*/
	bind(this->welcomeSocket, (struct sockaddr *) &serverAddr,
			sizeof(serverAddr));
}

Servidor::~Servidor() {
}

void Servidor::autenticar(string nombre, string contrasenia,
		list<string>& usuarios) {

	string linea, csvItem;
	string nombreCapturado, contraseniaCapturada;
	int nroItem;
	bool autenticacionOK = false;

	ifstream myfile(this->nombreArchivo);

	if (myfile.is_open()) {

		while (getline(myfile, linea)) {
			nroItem = 0;
			istringstream lineaActual(linea);

			while (getline(lineaActual, csvItem, ',')) {

				if (nroItem == 0) {
					nombreCapturado = csvItem;
					usuarios.push_back(csvItem);
				}
				if (nroItem == 1)
					contraseniaCapturada = csvItem;
				nroItem++;
			}
			if ((strcmp(nombre.c_str(), nombreCapturado.c_str()) == 0)
					&& (strcmp(contrasenia.c_str(),
							contraseniaCapturada.c_str()) == 0)) {
				usuarios.remove(nombreCapturado);
				autenticacionOK = true;
			}
		}
	}
	myfile.close();

	list<string>* users = &usuarios;

	if (autenticacionOK) {
		cout << "Autenticación OK wachin" << endl;
	} else {
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

void Servidor::aceptarConexiones() {
	string nombre;
	string pass;
	char datosRecibidos[1024];
	this->addr_size = sizeof serverStorage;
	cout << "Adentro de aceptarConexiones" << endl;
	this->socketServer = accept(welcomeSocket,
			(struct sockaddr *) &this->serverStorage, &this->addr_size);
	recv(socketServer, datosRecibidos, 1024, 0);
	cout << "Datos recibidos: " << datosRecibidos << endl;
	splitDatos(datosRecibidos, &nombre, &pass);
	this->autenticar(nombre, pass, this->usuarios);
}

int Servidor::aceptarConexion() {
	//esta funcion acepta las conexiones para cada cliente que lo solicita si es autenticado y devuelve el socket de dicho cliente.
	string nombre, pass;
	char buffer[1024];
	char datosRecibidos[1024];

	this->addr_size = sizeof serverStorage;
	cout << "Escuchando conexiones entrantes.." << endl;
	int socketCliente;
	socketCliente = accept(welcomeSocket,
			(struct sockaddr *) &this->serverStorage, &this->addr_size);

	recv(socketCliente, datosRecibidos, 1024, 0);
	cout << "Datos recibidos: " << datosRecibidos << endl;
	splitDatos(datosRecibidos, &nombre, &pass);

	this->autenticar(nombre, pass, this->usuarios);
	char* resultadoDeLaAutenticacion;
	this->cantClientesConectados += 1;

	if (this->usuarios.empty()) {
		resultadoDeLaAutenticacion = "No se pudo autenticar";
		// si no se puede autenticar debe desconectarse al usuario
	} else
		resultadoDeLaAutenticacion = "Autenticacion OK";

	//strcpy(buffer, resultadoDeLaAutenticacion);
	strcpy(buffer, this->serializarLista(this->usuarios));
	/*for (list<string>::iterator i = usuarios.begin(); i != usuarios.end(); i++) {
	 //cout << (*i) << endl;
	 strcat(buffer, strdup((*i).c_str()));
	 strcat(buffer,",");
	 }*/

	string buff = buffer;
	cout << buff << endl;
	//le respondo al cliente un mensaje que dice si se pudo autenticar o no. hay que cambiar esto para qe le mande al cliente la lista que corresponde.
	//send(socketCliente, buffer, strlen(resultadoDeLaAutenticacion) + 1, 0);
	send(socketCliente, buffer, 1024, 0);
	//FALTA HACER QUE UNA VEZ QUE SE AUTENTIQUE, ESTA FUNCION HAGA SEND AL CLIENTE DE LA LISTA DE USUARIOS DISPONIBLES.
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
	char datosMensaje[1024];
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

const char* Servidor::serializarLista(list<string> datos) {
	char* buffer;
	for (list<string>::iterator i = datos.begin(); i != datos.end(); i++) {
		cout << (*i) << endl;
		strcat(buffer, strdup((*i).c_str()));
		strcat(buffer, ",");
	}
	return buffer;
}
