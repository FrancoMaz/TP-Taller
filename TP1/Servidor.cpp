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

Servidor::Servidor(string nombreArchivoDeUsuarios, int puerto) {
	this->puerto = puerto;
}

Servidor::~Servidor() {
}

Servidor::Servidor(int puerto){
	this->puerto = puerto;
}

list<Cliente> Servidor::autenticar(string nombre, string contrasenia) {

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
