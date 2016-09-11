/*
 * Servidor.cpp
 *
 *  Created on: 28 ago. 2016
 *      Author: franco
 */

#include "Servidor.h"
#include <string.h>
using namespace std;

Servidor::Servidor(char* nombreArchivoDeUsuarios, int puerto, int modoLogger) {
	this->puerto = puerto;
	this->nombreArchivo = nombreArchivoDeUsuarios;
	this->logger = new Logger(modoLogger);
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
	bind(this->welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
	this->datosUsuarios = new list<Datos>();
	this->listaMensajesProcesados = new list<MensajesProcesados>();
	this->guardarDatosDeUsuarios();
	mensajeStream << "Se creó correctamente el servidor en el puerto: " << puerto << ", ip: 192.168.1.10" << "\n";
	this->guardarLog(mensajeStream);
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

	mensajeStream << "Se leyeron los datos de los usuarios desde el archivo usuarios.csv \n";
	this->guardarLog(mensajeStream);
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
		mensajeStream << "Autenticación OK \n";
		this->guardarLog(mensajeStream);
	} else {
		usuarios.clear();
		cout << "Error de autenticación, no nos hackees wachin" << endl;
		mensajeStream << "Error de autenticación \n";
		this->guardarLog(mensajeStream);
	}
}

list<Cliente> Servidor::obtenerClientes() {

}

void Servidor::guardarLog(stringstream &mensaje) {
	string msj = mensajeStream.str();
	this->logger->escribir(msj);
}

list<Mensaje> Servidor::obtenerMensajes(Cliente cliente) {

}

void Servidor::crearMensaje(Mensaje mensaje) {
	this->colaMensajesNoProcesados.push(mensaje);
	this->procesarMensaje(mensaje); //Por ahora invoco a este metodo aca pero deberia ir en un while(1) dentro del mainServidor, para que la cola se mensajes no procesados se procese infinitamente
}

void Servidor::procesarMensaje(Mensaje mensaje) {
	if (!colaMensajesNoProcesados.empty()) {
		for (list<MensajesProcesados>::iterator usuarioActual = listaMensajesProcesados->begin(); usuarioActual != listaMensajesProcesados->end(); usuarioActual++) {
				MensajesProcesados listaMensajes;
				listaMensajes = *usuarioActual;
				if (listaMensajes.destinatario == mensaje.getDestinatario()){
					listaMensajes.mensajes->push(mensaje);
					cout << "Procesado mensaje para " << listaMensajes.destinatario << endl;
					}
		}
	}
}

void Servidor::comenzarEscucha() {
	//Metodo que pone al servidor a escuchar si alguien requiere algo.
	this->escuchando = (listen(this->welcomeSocket, MAX_CANT_CLIENTES) == 0);
	mensajeStream << "El servidor está escuchando... \n";
	this->guardarLog(mensajeStream);
	cout << "Escuchando conexiones entrantes.." << endl;
}


int Servidor::aceptarConexion() {
	//esta funcion acepta las conexiones para cada cliente que lo solicita si es autenticado y devuelve el socket de dicho cliente.
	string nombre, pass;
	char buffer[1024];
	char datosRecibidos[1024];

	this->addr_size = sizeof serverStorage;
	int socketCliente = accept(welcomeSocket, (struct sockaddr *) &this->serverStorage, &this->addr_size);
	recv(socketCliente, datosRecibidos, 1024, 0);

	cout << "Datos recibidos: " << datosRecibidos << endl;
	mensajeStream << "Datos recibidos: " << datosRecibidos << "\n";
	this->guardarLog(mensajeStream);

	splitDatos(datosRecibidos, &nombre, &pass);
	this->autenticar(nombre, pass, usuarios);
	char* resultadoDeLaAutenticacion;
	this->cantClientesConectados += 1;

	if (usuarios.empty()) {
		strcpy(buffer, "Desconectar");
		mensajeStream << "Se desconecta al usuario " << nombre << " del servidor porque falló la autenticación... \n";
		this->guardarLog(mensajeStream);
		send(socketCliente, buffer, 1024, 0);
	} else {
		strcpy(buffer, this->serializarLista(usuarios).c_str());
		string bufferS = buffer;
		mensajeStream << "Enviándole al cliente " << nombre << " la lista de usuarios disponibles \n";
		this->guardarLog(mensajeStream);
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

list<string> Servidor::agregarDestinatarios(char* remitente){
	list<string> destinatarios;
	for (list<Servidor::Datos>::iterator datoActual = datosUsuarios->begin(); datoActual != datosUsuarios->end(); datoActual++) {
		Datos usuario;
		usuario = *datoActual;
		if (usuario.nombre != remitente){
			destinatarios.push_back(usuario.nombre);
				}
			}
	return destinatarios;
}

