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

list<string>* Servidor::autenticar(string nombre, string contrasenia) {

	string linea, csvItem;
	string nombreCapturado, contraseniaCapturada;
	int nroItem;
	bool autenticacionOK = false;
	list<string> usuarios;
	list<string>* users = &usuarios;
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
				if (nroItem == 1) contraseniaCapturada = csvItem;
				nroItem++;
			}
			if ((strcmp(nombre.c_str(), nombreCapturado.c_str()) == 0) && (strcmp(contrasenia.c_str(), contraseniaCapturada.c_str()) == 0)) {
				usuarios.remove(nombreCapturado);
				autenticacionOK = true;
			}
		}
	}
	myfile.close();

	if (autenticacionOK) {
		cout << "Autenticación OK wachin" << endl;
		for (list<string>::const_iterator i = usuarios.begin(); i != usuarios.end(); i++) {
			printf("%s\n", i->c_str());
		}
		return users;
	} else {
		cout << "Error de autenticación, no nos hackees wachin" << endl;
		return new list<string>();
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
	/*char buffer[1024];
	 struct sockaddr_storage sender;
	 socklen_t sendsize = sizeof(sender);*/

	//Metodo que pone al servidor a escuchar si alguien requiere algo.
	this->escuchando = (listen(this->welcomeSocket, 6) == 0);
	if (escuchando) {
		printf("Listening\n");
		//string user, pass;
		//recvfrom(newSocket, buffer, 1024, 0,(struct sockaddr*)&sender, &sendsize);
		//cout << buffer << endl;
	} else {
		printf("Error\n");
	}
}

void Servidor::aceptarConexiones() {
	string nombre;
	string pass;
	char datosRecibidos[1024];
	int newSocket;
	this->addr_size = sizeof serverStorage;
	cout << "Adentro de aceptarConexiones" << endl;
	newSocket = accept(welcomeSocket, (struct sockaddr *) &this->serverStorage, &this->addr_size);
	recv(newSocket, datosRecibidos, 1024, 0);
	cout << datosRecibidos << "Datos recibidos" << endl;
	splitDatos(datosRecibidos, &nombre, &pass);
	this->autenticar(nombre, pass);
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
