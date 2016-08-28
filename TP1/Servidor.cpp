/*
 * Servidor.cpp
 *
 *  Created on: 28 ago. 2016
 *      Author: franco
 */

#include "Servidor.h"
#include <string>
using namespace std;

Servidor::Servidor(string nombreArchivoDeUsuarios, int puerto) {
	this->puerto = puerto;
}

Servidor::~Servidor() {
}

List<Cliente> Cliente::autenticar(string nombre, string contrasenia) {

}

List<Cliente> Cliente::obtenerClientes() {

}

void Cliente::guardarLog() {
//Guarda toda la actividad en un archivo de texto

}

List<string> Cliente::obtenerMensajes(Cliente cliente) {

}

void crearMensaje(Cliente clienteOrigen, Cliente clienteDestinatario) {

}
