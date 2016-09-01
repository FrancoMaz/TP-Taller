/*
 * Servidor.cpp
 *
 *  Created on: 28 ago. 2016
 *      Author: franco
 */

#include "Servidor.h"
#include "Cliente.h"
#include <string>
using namespace std;

#define PATH "lala"

Servidor::Servidor(string nombreArchivoDeUsuarios, int puerto) {
	this->puerto = puerto;
	archivoUsuarios = fopen((const char*)PATH + (const char*)nombreArchivoDeUsuarios + (const char*)".csv", "r");
	this->nombreArchivo = nombreArchivoDeUsuarios;
}

Servidor::~Servidor() {
}

Servidor::Servidor(int puerto){
	this->puerto = puerto;
}

list<Cliente> Servidor::autenticar(string nombre, string contrasenia) {

	string linea, csvItem;
	string nombreCapturado, contraseniaCapturada;
	int nroItem;
	bool autenticacionOK = false;

	ifstream archivoCSV(this->nombreArchivo);

	if (archivoCSV.is_open()) {

			while (getline(archivoCSV,linea) && !autenticacionOK) {
				nroItem = 0;
				istringstream lineaActual(linea);

				while(getline(lineaActual,csvItem,',')) {

					if (nroItem == 0) nombreCapturado = csvItem;
					if (nroItem == 1) contraseniaCapturada = csvItem;
					nroItem++;
				}
				if ((nombre == nombreCapturado) && (contrasenia == contraseniaCapturada)) {
					autenticacionOK = true;
				}
			}
	}
	archivoCSV.close();

	if (autenticacionOK) cout << "Autenticación OK wachin" << endl;
	else cout << "Error de autenticación, no nos hackees wachin" << endl;
}

list<Cliente> Servidor::obtenerClientes() {

}

void Servidor::guardarLog() {
//Guarda toda la actividad en un archivo de texto

}

list<string> Servidor::obtenerMensajes(Cliente cliente) {

}

void crearMensaje(Cliente clienteOrigen, Cliente clienteDestinatario) {

}

void comenzarEscucha(){
	//Metodo que pone al servidor a escuchar si alguien requiere algo.
}

void finalizarEscucha(){
	//Metodo que finaliza la escucha del servidor.
}
