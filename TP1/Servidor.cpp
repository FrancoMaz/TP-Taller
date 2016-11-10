/*
 * Servidor.cpp
 *
 *  Created on: 28 ago. 2016
 *      Author: franco
 */

#include "Servidor.h"
#include <string.h>
#include <stdlib.h>
#include "LTimer.h"
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
	//this->serverAddr.sin_addr.s_addr = inet_addr("192.168.1.12");
	this->serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	//this->serverAddr.sin_addr.s_addr = inet_addr("10.1.77.13");
	/* Set all bits of the padding field to 0 */
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
	/*---- Bind the address struct to the socket ----*/
	bind(this->welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
	this->datosUsuarios = new list<Datos>();
	this->jugadores = new vector<Jugador*>();
	this->listaMensajesProcesados = new list<MensajesProcesados>();
	stringstream ss;
	ss << puerto;
	mensaje = "Se creó correctamente el servidor en el puerto: " + ss.str() + ", ip: 192.168.1.10" + "\n";
	this->guardarLog(mensaje, DEBUG);
	this->guardarDatosDeUsuarios();
	this->guardarDatosDeConfiguracion();
	vectorEquipos = {"rojo", "verde", "amarillo", "azul"};
	posicionXInicial = 20;
	posicionVector = 0;
	camara.x = 0;
	camara.y = 0;
	for (int i = 0; i < handshake->getImagenes().size(); i++)
	{
		pair<int,int> abscisas;
		abscisas.first = 0;
		abscisas.second = 0;
		abscisasCapas.push_back(abscisas);
	}
}

Servidor::~Servidor() {
}

void Servidor::guardarDatosDeConfiguracion() {
	string path;
	cin.ignore();
	cout << "Ingrese el path del archivo de configuracion" << endl;
	getline(cin,path);
	//cin >> path;

	if (this->existeArchivo(path))
	{
	   this->parser = new XmlParser(path);
	}
	else {
			if(path.empty()){
				cout << "No se ingreso ninguna ruta. Se cargara el xml por defecto" << endl;
			}
			else {
				cout << "No existe el archivo ingresado. Se cargara el xml por defecto" << endl;
			}
			this->parser = new XmlParser("Recursos/configuration.xml");
	}
	vector<ImagenDto*> escenario = this->parser->getEscenario();
	vector<SetDeSpritesDto*> setDeSprites = this->parser->getSprites();
	pair<const char*, const char*> ventana = this->parser->getTamanioVentana();
	const char* cantidadMaximaJugadores = this->parser->getCantidadMaximaDeJugadores();

	ImagenDto* imagenTemporal;
	for (int i = 0; i < escenario.size(); i++) {
		for (int j = 0; j < escenario.size() - 1; j++) {
			if (escenario.at(j)->getZIndex() > escenario.at(j+1)->getZIndex()) {
				imagenTemporal = escenario.at(j);
				escenario.at(j) = escenario.at(j+1);
				escenario.at(j+1) = imagenTemporal;
			}
		}
	}

	this->handshake = new Handshake(escenario, setDeSprites, ventana.first, ventana.second, cantidadMaximaJugadores);
}

bool Servidor::existeArchivo(string fileName) {
	ifstream infile(fileName.c_str());
	return infile.good();
}

void Servidor::enviarHandshake(int socket, char* cliente){
	string handshake = this->parser->serializarEscenario();
	handshake += this->parser->serializarSetDeSprites();
	handshake += this->parser->serializarVentana();
	handshake += "#@";
	int largo = strlen(handshake.c_str());
	std::ostringstream oss;
	oss << largo;
	string largoString = oss.str();
	char buffer[BUFFER_MAX_SIZE];
	int inicio = 0;
	int ok;
	if (largo > BUFFER_MAX_SIZE) {
		while (BUFFER_MAX_SIZE < (largo - inicio)) {
			string mensajeSpliteado = handshake.substr(inicio, BUFFER_MAX_SIZE);
			strcpy(buffer, mensajeSpliteado.c_str());
			ok = send(socket, buffer, strlen(buffer), 0);
			inicio += BUFFER_MAX_SIZE;
		}
		string mensajeSpliteado = handshake.substr(inicio, largo);
		strcpy(buffer, mensajeSpliteado.c_str());
	} else {
		strcpy(buffer, handshake.c_str());
	}
	//pthread_mutex_lock(&servidor->mutexSocket);
	ok = send(socket, buffer, strlen(buffer), 0);
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
				}
				if (nroItem == 1) {
					datosCapturados.contrasenia = csvItem;
				}
				nroItem++;
			}
			queue<Mensaje>* colaMensajes = new queue<Mensaje>;
			mensajesProcesados.mensajes = colaMensajes;
			datosUsuarios->push_back(datosCapturados);
		}

		myfile.close();
	}

}

bool Servidor::contieneJugador(string nombre)
{
	bool estaContenido = false;
	for (int i=0; i <jugadores->size(); i++)
	{
		if (jugadores->at(i)->getNombre() == nombre)
		{
			estaContenido = true;
			break;
		}
	}
	return estaContenido;
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
			if (jugadores->empty() || !this->contieneJugador(usuario.nombre))
			{
				Jugador* jugador = new Jugador(usuario.nombre, this->vectorEquipos.at(posicionVector), posicionXInicial);
				jugador->setConectado();
				pthread_mutex_lock(&mutexVectorJugadores);
				jugadores->push_back(jugador);
				pthread_mutex_unlock(&mutexVectorJugadores);
				MensajesProcesados mensajesProcesados;
				mensajesProcesados.destinatario = usuario.nombre;
				queue<Mensaje>* colaMensajes = new queue<Mensaje>;
				mensajesProcesados.mensajes = colaMensajes;
				listaMensajesProcesados->push_back(mensajesProcesados);
			}
			else
			{
				Jugador* jugador = this->obtenerJugador(usuario.nombre);
				jugador->setConectado();
				jugador->setSprite("Jugador_" + jugador->getEquipo());
			}
		} else {
			usuarios.push_back(usuario.nombre);
		}
	}
	if (autenticacionOK) {
		cout << "Autenticación OK" << endl;
	} else {
		usuarios.clear();
		cout << "Error de autenticación: usuario y/o clave incorrectos" << endl;
		string msj = "Error de autenticación: usuario y/o clave incorrectos \n";
		this->guardarLog(msj, INFO);
	}
	this->posicionVector += 1;
	this->posicionXInicial += 84;
	if (this->posicionVector > this->vectorEquipos.size())
	{
		this->posicionVector = 0;
	}
}


void Servidor::guardarLog(string mensaje, const int nivelDeLog) {
	pthread_mutex_lock(&mutexLog);
	this->logger->escribir(mensaje, nivelDeLog);
	pthread_mutex_unlock(&mutexLog);
}


void Servidor::crearMensaje(Mensaje mensaje) {
	this->colaMensajesNoProcesados.push(mensaje);
}

list<Servidor::MensajesProcesados>* Servidor::getListaMensajesProcesados()
{
	return listaMensajesProcesados;
}

/*void* Servidor::actualizarPosiciones(void* arg)
{
	ParametrosServidor parametrosServidor = *(ParametrosServidor*) arg;
	Mensaje mensajeAProcesar = parametrosServidor.mensajeAProcesar;
	Servidor* servidor = parametrosServidor.servidor;
	Mensaje* mensajeCamara;
	string mensajeCamaraString;
	LTimer timer;
	bool jugadorSalto;
	do {
		timer.start();
		int frameTicks = timer.getTicks();
		string mensajeJugadorPosActualizada = "";
		//pthread_mutex_lock(&servidor->mutexVectorJugadores);
		Jugador* jugador = servidor->obtenerJugador(mensajeAProcesar.getRemitente());
		jugador->actualizarPosicion(mensajeAProcesar.deserializar(mensajeAProcesar.getTexto()),mensajeAProcesar.sePresionoTecla(),servidor->camara);
		jugadorSalto = jugador->salto();
		int anchoSprite = servidor->getAnchoSprite(jugador->getSpriteAEjecutar());
		pair<int,int> posicionesExtremos = servidor->obtenerPosicionesExtremos();
		bool necesitaCambiarCamara = jugador->chequearCambiarCamara(servidor->camara, atoi(servidor->handshake->getAncho().c_str()), posicionesExtremos, anchoSprite);
		if (necesitaCambiarCamara)
		{
			if (servidor->camara.x > atoi(servidor->handshake->getImagenes().at(0)->getAncho().c_str())){
				servidor->camara.x = 0;
				for (int i = 0; i < servidor->abscisasCapas.size(); i++)
				{
					servidor->abscisasCapas.at(i).first = 0;
				}
				for (int i = 0; i < servidor->jugadores->size(); i++)
				{
					servidor->jugadores->at(i)->resetearPosicion(atoi(servidor->handshake->getImagenes().at(0)->getAncho().c_str()));
				}
			} else
				{
				servidor->camara.x += jugador->getVelocidadX();
				//servidor->camara.x = (jugador->getPosicion().first) - (atoi(servidor->handshake->getAncho().c_str()))/2;
				for (int i = 0; i < servidor->abscisasCapas.size(); i++)
				{
					if (i == 0)
					{
						servidor->abscisasCapas.at(i).first = servidor->camara.x;
					}
					else
					{	servidor->abscisasCapas.at(i).second += jugador->getVelocidadX();
						servidor->abscisasCapas.at(i).first = abs((servidor->abscisasCapas.at(i).second)*(atoi(servidor->handshake->getImagenes().at(i)->getAncho().c_str()) - atoi(servidor->handshake->getAncho().c_str()))/(atoi(servidor->handshake->getImagenes().at(0)->getAncho().c_str()) - atoi(servidor->handshake->getAncho().c_str())));
						if (servidor->abscisasCapas.at(i).first > atoi(servidor->handshake->getImagenes().at(i)->getAncho().c_str()))
						{
							servidor->abscisasCapas.at(i).first = 0;
							servidor->abscisasCapas.at(i).second = 0;
						}
					}
				}
				}
			//mensajeCamaraString = "1|" + to_string(jugador->getVelocidadX()) + "#";
			mensajeCamaraString = "1|" + to_string(servidor->camara.x) + "|" + to_string(servidor->camara.y) + "|" + servidor->serializarCapas() + "#";
			mensajeCamara = new Mensaje(jugador->getNombre(),"Todos",mensajeCamaraString);
		}
		mensajeJugadorPosActualizada = jugador->getStringJugador();
		//pthread_mutex_unlock(&servidor->mutexVectorJugadores);
		servidor->encolarMensajeProcesadoParaCadaCliente(mensajeAProcesar,mensajeJugadorPosActualizada);
		if (necesitaCambiarCamara)
		{
			servidor->encolarMensajeProcesadoParaCadaCliente(*mensajeCamara,mensajeCamaraString);
		}
		if( frameTicks < 25 )
		{
			//Wait remaining time
			SDL_Delay( 25 - frameTicks );
		}
	} while (jugadorSalto);
}*/
/*
=======

>>>>>>> 7141cadd9db71637618ff321e8c9e0314344fbbd
void* Servidor::actualizarPosicionesJugador(void* arg)
{
	//ParametrosActPosicion* parametrosActPosicion = (ParametrosActPosicion*) arg;
	//Servidor* servidor = (Servidor*)arg;
	//string nombreJugador = "jochi";
	//Jugador* jugador = parametrosActPosicion->jugador;
	//Servidor* servidor = parametrosActPosicion->servidor;
	//Jugador* jugador = servidor->obtenerJugador(nombreJugador);
	ParametrosMovimiento* parametros = (ParametrosMovimiento*)arg;
	Servidor* servidor = parametros->servidor;
	Jugador* jugador = parametros->jugador;
	Mensaje* mensajeCamara;
	string mensajeCamaraString;
	string mensajeJugadorPosActualizada = "";
	while (jugador->getConectado()){
		//cout << "parametros: " << parametrosActPosicion << endl;
		cout<<"posicion de memoria: " << servidor << endl;
		pthread_mutex_lock(&servidor->mutexVectorJugadores);
		jugador->mover(servidor->camara);
		pair<int,int> posicionesExtremos = servidor->obtenerPosicionesExtremos();
		int anchoSprite = servidor->getAnchoSprite(jugador->getSpriteAEjecutar());
		bool necesitaCambiarCamara = jugador->chequearCambiarCamara(servidor->camara, atoi(servidor->handshake->getAncho().c_str()), posicionesExtremos, anchoSprite);
		if (necesitaCambiarCamara)
		{
			if (servidor->camara.x > atoi(servidor->handshake->getImagenes().at(0)->getAncho().c_str())){
				servidor->camara.x = 0;
				for (int i = 0; i < servidor->abscisasCapas.size(); i++)
				{
					servidor->abscisasCapas.at(i).first = 0;
				}
				for (int i = 0; i < servidor->jugadores->size(); i++)
				{
					servidor->jugadores->at(i)->resetearPosicion(atoi(servidor->handshake->getImagenes().at(0)->getAncho().c_str()));
				}
			} else
				{
					servidor->camara.x += jugador->getVelocidadX();
					//camara.x = (jugador->getPosicion().first) - (atoi(handshake->getAncho().c_str()))/2;
					for (int i = 0; i < servidor->abscisasCapas.size(); i++)
					{
						if (i == 0)
						{
							servidor->abscisasCapas.at(i).first = servidor->camara.x;
						}
						else
						{	servidor->abscisasCapas.at(i).second += jugador->getVelocidadX();
							servidor->abscisasCapas.at(i).first = abs((servidor->abscisasCapas.at(i).second)*(atoi(servidor->handshake->getImagenes().at(i)->getAncho().c_str()) - atoi(servidor->handshake->getAncho().c_str()))/(atoi(servidor->handshake->getImagenes().at(0)->getAncho().c_str()) - atoi(servidor->handshake->getAncho().c_str())));
							if (servidor->abscisasCapas.at(i).first > atoi(servidor->handshake->getImagenes().at(i)->getAncho().c_str()))
							{
								servidor->abscisasCapas.at(i).first = 0;
								servidor->abscisasCapas.at(i).second = 0;
							}
						}
					}
				}
			mensajeCamaraString = "1|" + to_string(servidor->camara.x) + "|" + to_string(servidor->camara.y) + "|" + servidor->serializarCapas() + "#";
			mensajeCamara = new Mensaje(jugador->getNombre(),"Todos",mensajeCamaraString);
		}
		mensajeJugadorPosActualizada = jugador->getStringJugador();
		Mensaje* mensajeAProcesar = new Mensaje();
		mensajeAProcesar->setRemitente(jugador->getNombre());
		pthread_mutex_unlock(&servidor->mutexVectorJugadores);
		mensajeAProcesar->setDestinatario("Todos");
		servidor->encolarMensajeProcesadoParaCadaCliente(*mensajeAProcesar,mensajeJugadorPosActualizada);
		if (necesitaCambiarCamara)
		{
			servidor->encolarMensajeProcesadoParaCadaCliente(*mensajeCamara,mensajeCamaraString);
		}
		usleep(50000);
	}
}
*/
Jugador* Servidor::obtenerJugador(string nombre){
	for (int i = 0; i < jugadores->size(); i++)
	{
		Jugador* jugador = jugadores->at(i);
		if (jugador->getNombre() == nombre)
		{
			return jugador;
			break;
		}
	}
}

pair<int,int> Servidor::obtenerPosicionesExtremos()
{
	int posicionX = jugadores->at(0)->getPosicion().first;
	int posicionMasAdelante = camara.x;
	int posicionMasAtras = camara.x + atoi(handshake->getAncho().c_str());
	for (int i = 0; i < jugadores->size(); i++)
	{
		Jugador* jugador = jugadores->at(i);
		if (jugador->getPosicion().first <= posicionMasAtras && jugador->getConectado())
		{
			posicionMasAtras = jugadores->at(i)->getPosicion().first;
		}
		else if (jugador->getPosicion().first >= posicionMasAdelante && jugador->getConectado())
		{
			posicionMasAdelante = jugadores->at(i)->getPosicion().first;
		}
	}
	pair<int,int> posiciones;
	posiciones.first = posicionMasAtras;
	posiciones.second = posicionMasAdelante;
	return posiciones;
}

int Servidor::getAnchoSprite(string sprite)
{
	int ancho = 0;
	vector<SetDeSpritesDto*> setsSprites = handshake->getSprites();
	for (int i = 0; i < setsSprites.size(); i++){
		vector<SpriteDto*> listaSprites = setsSprites.at(i)->getSprites();
		for (int i = 0; i < listaSprites.size(); i++)
		{
			if (sprite == listaSprites.at(i)->getID())
			{
				ancho = atoi(listaSprites.at(i)->getAncho().c_str());
			}
		}
	}
	return ancho;
}

string Servidor::serializarCapas()
{
	string capas = "";
	for (int i = 0; i < abscisasCapas.size(); i++)
	{
		capas += to_string(abscisasCapas.at(i).first);
		capas += ",";
		capas += to_string(abscisasCapas.at(i).second);
		capas += "|";
	}
	return capas;
}

void Servidor::procesarMensajes() {
	Mensaje* mensajeCamara;
	string mensajeCamaraString;
	//usleep(5000);
	if (!colaMensajesNoProcesados.empty()) {
		pthread_t threadSalto;
		pthread_mutex_lock(&mutexColaNoProcesados);
		Mensaje mensajeAProcesar = colaMensajesNoProcesados.front();
		colaMensajesNoProcesados.pop();
		pthread_mutex_unlock(&mutexColaNoProcesados);
		if (mensajeAProcesar.getTexto() == "Tecla Arriba")
		{
			/*ParametrosServidor parametrosServidor;
			parametrosServidor.mensajeAProcesar = mensajeAProcesar;
			parametrosServidor.servidor = this;

			//this->actualizarPosicionesSalto(mensajeAProcesar);
			pthread_create(&threadSalto,NULL,&actualizarPosiciones,&parametrosServidor);
			pthread_detach(threadSalto);*/

			pthread_mutex_lock(&mutexVectorJugadores);
			Jugador* jugador = this->obtenerJugador(mensajeAProcesar.getRemitente());
			if (jugador->getConectado()){
				jugador->setMov(mensajeAProcesar.deserializar(mensajeAProcesar.getTexto()), mensajeAProcesar.sePresionoTecla());
			}
			pthread_mutex_unlock(&mutexVectorJugadores);
		}
		else if (mensajeAProcesar.getTexto() == "R"){
			string mensajeJugadorPosActualizada = "";
			camara.x = 0;
			camara.y = 0;
			for (int i = 0; i < abscisasCapas.size(); i++)
			{
				abscisasCapas.at(i).first = 0;
				abscisasCapas.at(i).second = 0;
			}
			int xInicial = 20;
			for (int i=0; i < jugadores->size(); i++)
			{
				Jugador* jugador = jugadores->at(i);
				mensajeAProcesar.setRemitente(jugador->getNombre());
				jugador->setPosicion(xInicial);
				xInicial += 84;
				jugador->setSprite("Jugador",true,"Normal");
				mensajeJugadorPosActualizada = jugador->getStringJugador();
				encolarMensajeProcesadoParaCadaCliente(mensajeAProcesar,mensajeJugadorPosActualizada);
			}
			mensajeCamaraString = "1|" + to_string(camara.x) + "|" + to_string(camara.y) + "|" + this->serializarCapas() + "#";
			mensajeCamara = new Mensaje(mensajeAProcesar.getRemitente(),"Todos",mensajeCamaraString);
			encolarMensajeProcesadoParaCadaCliente(*mensajeCamara,mensajeCamaraString);
		}
		else
		{
			pthread_mutex_lock(&mutexVectorJugadores);
			Jugador* jugador = this->obtenerJugador(mensajeAProcesar.getRemitente());
			if (jugador->getConectado()){
				jugador->setMov(mensajeAProcesar.deserializar(mensajeAProcesar.getTexto()), mensajeAProcesar.sePresionoTecla());
			}
			pthread_mutex_unlock(&mutexVectorJugadores);
			/*string mensajeJugadorPosActualizada = "";
			pthread_mutex_lock(&mutexVectorJugadores);
			Jugador* jugador = this->obtenerJugador(mensajeAProcesar.getRemitente());
			if (jugador->getConectado())
			{
				jugador->actualizarPosicion(mensajeAProcesar.deserializar(mensajeAProcesar.getTexto()),mensajeAProcesar.sePresionoTecla(), camara);
			}
			pair<int,int> posicionesExtremos = this->obtenerPosicionesExtremos();
			int anchoSprite = this->getAnchoSprite(jugador->getSpriteAEjecutar());
			bool necesitaCambiarCamara = jugador->chequearCambiarCamara(this->camara, atoi(handshake->getAncho().c_str()), posicionesExtremos, anchoSprite);
			if (necesitaCambiarCamara)
			{
				if (camara.x > atoi(handshake->getImagenes().at(0)->getAncho().c_str())){
					camara.x = 0;
					for (int i = 0; i < abscisasCapas.size(); i++)
					{
						abscisasCapas.at(i).first = 0;
					}
					for (int i = 0; i < jugadores->size(); i++)
					{
						jugadores->at(i)->resetearPosicion(atoi(handshake->getImagenes().at(0)->getAncho().c_str()));
					}
				} else
					{
						camara.x += jugador->getVelocidadX();
						//camara.x = (jugador->getPosicion().first) - (atoi(handshake->getAncho().c_str()))/2;
						for (int i = 0; i < abscisasCapas.size(); i++)
						{
							if (i == 0)
							{
								abscisasCapas.at(i).first = camara.x;
							}
							else
							{	abscisasCapas.at(i).second += jugador->getVelocidadX();
								abscisasCapas.at(i).first = abs((abscisasCapas.at(i).second)*(atoi(handshake->getImagenes().at(i)->getAncho().c_str()) - atoi(handshake->getAncho().c_str()))/(atoi(handshake->getImagenes().at(0)->getAncho().c_str()) - atoi(handshake->getAncho().c_str())));
								if (abscisasCapas.at(i).first > atoi(handshake->getImagenes().at(i)->getAncho().c_str()))
								{
									abscisasCapas.at(i).first = 0;
									abscisasCapas.at(i).second = 0;
								}
							}
						}
					}
				mensajeCamaraString = "1|" + to_string(camara.x) + "|" + to_string(camara.y) + "|" + this->serializarCapas() + "#";
				mensajeCamara = new Mensaje(jugador->getNombre(),"Todos",mensajeCamaraString);
			}
			mensajeJugadorPosActualizada = jugador->getStringJugador();
			pthread_mutex_unlock(&mutexVectorJugadores);
			encolarMensajeProcesadoParaCadaCliente(mensajeAProcesar,mensajeJugadorPosActualizada);
			if (necesitaCambiarCamara)
			{
				encolarMensajeProcesadoParaCadaCliente(*mensajeCamara,mensajeCamaraString);
			}
			*/
		}
	}
}

void Servidor::encolarMensajeProcesadoParaCadaCliente(Mensaje mensajeAProcesar, string mensajeJugadorPosActualizada){
	Mensaje* mensajePosicionActualizada;
	for (list<MensajesProcesados>::iterator usuarioActual = listaMensajesProcesados->begin();usuarioActual != listaMensajesProcesados->end();usuarioActual++)
	{
		MensajesProcesados listaMensajes;
		listaMensajes = *usuarioActual;
		if (listaMensajes.destinatario == mensajeAProcesar.getDestinatario() or mensajeAProcesar.getDestinatario() == "Todos")
		{
			pthread_mutex_lock(&mutexVectorJugadores);
			Jugador* jugador = this->obtenerJugador(listaMensajes.destinatario);
			if (jugador->getConectado())
			{
				mensajePosicionActualizada = new Mensaje(mensajeAProcesar.getRemitente(),listaMensajes.destinatario,mensajeJugadorPosActualizada);
				pthread_mutex_lock(&mutexListaProcesados);
				listaMensajes.mensajes->push(*mensajePosicionActualizada);
				pthread_mutex_unlock(&mutexListaProcesados);
			}
			pthread_mutex_unlock(&mutexVectorJugadores);
		}
	}
}

void Servidor::comenzarEscucha() {
//Metodo que pone al servidor a escuchar si alguien requiere algo.
	this->escuchando = (listen(this->welcomeSocket, MAX_CANT_CLIENTES) == 0);
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
	struct timeval timeout;
	timeout.tv_sec = 5;
	timeout.tv_usec = 0;
	if (setsockopt (socketCliente, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
	{
		cout << "No se pudo setear el timeout del recv del socket" << endl;
	}
	if (setsockopt (socketCliente, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
	{
		cout << "No se pudo setear el timeout del send del socket" << endl;
	}
	//pthread_mutex_lock(&mutexSocket);
	int ok = recv(socketCliente, datosRecibidos, BUFFER_MAX_SIZE, 0);
	if (ok < 0){
		this->guardarLog("No se pudieron recibir los datos para la conexion.",INFO);
		this->guardarLog("ERROR: Problema con el recv del socket.",DEBUG);
		cli.first = ok;
		cli.second = "";
		return cli;
	}
	cout << "Datos recibidos: " << datosRecibidos << endl;

	splitDatos(datosRecibidos, &nombre, &pass);
	this->autenticar(nombre, pass, usuarios);
	char* resultadoDeLaAutenticacion;
    int okAutenticar = 1;
	if (usuarios.empty()) {
		okAutenticar = -1;
		strcpy(buffer, "Desconectar");
		ok = send(socketCliente, buffer, BUFFER_MAX_SIZE, 0);
		cli.first = -1;
		cli.second = "";
		return cli;
	}
	else {
		this->cantClientesConectados += 1;
		strcpy(buffer, this->serializarLista(usuarios).c_str());
		string bufferS = buffer;
		for (list<string>::iterator i = usuarios.begin(); i != usuarios.end();i++) {
			mensaje += (*i);
			mensaje += " ";
		}
		ok = send(socketCliente, buffer, BUFFER_MAX_SIZE, 0);
	}
	//pthread_mutex_unlock(&mutexSocket);
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

void Servidor::setThreadProceso(pthread_t thrProceso) {
	this->threadProcesador = thrProceso;
}

int Servidor::getCantConexiones() {
	return this->cantClientesConectados;
}

string Servidor::serializarLista(list<string> datos) {
	string buffer = "";
	for (list<string>::iterator i = datos.begin(); i != datos.end(); i++) {
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
string Servidor::traerMensajesProcesados(string nombreCliente) {

	queue<Mensaje>* colaDeMensajes;

	for (list<Servidor::MensajesProcesados>::iterator datoActual = listaMensajesProcesados->begin();
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
		mensajesConcatenados += mensaje.getRemitente();
		mensajesConcatenados += "|";
		mensajesConcatenados += mensaje.getTexto();
	}
	string lala = mensajesConcatenados.substr(0, mensajesConcatenados.length() -1);
	lala += "@";

	return lala;
}

void Servidor::restarCantidadClientesConectados(){
	this->cantClientesConectados -= 1;
}

vector<Jugador*>* Servidor::getJugadores()
{
	return jugadores;
}

void Servidor::setJugadorConectado(string nombre)
{
	Jugador* jugador = obtenerJugador(nombre);
	jugador->setConectado();
}

void Servidor::setJugadorDesconectado(string nombre)
{
	Jugador* jugador = this->obtenerJugador(nombre);
	jugador->setDesconectado();
}

vector<Jugador*>* Servidor::getJugadoresConectados()
{
	vector<Jugador*>* jugadoresConectados = new vector<Jugador*>();
	for (int i = 0; i < jugadores->size(); i++)
	{
		if (jugadores->at(i)->getConectado())
		{
			jugadoresConectados->push_back(jugadores->at(i));
		}
	}
	return jugadoresConectados;
}

string Servidor::getEstadoInicialSerializado()
{
	string estadoInicial = "";
	vector<Jugador*>* jugadoresConectados = getJugadoresConectados();
	for (int i = 0; i < jugadoresConectados->size(); i++)
	{
		estadoInicial += jugadoresConectados->at(i)->serializarInicio() + "#";
	}
	estadoInicial = estadoInicial + "camara|" + to_string(camara.x) + "|" + to_string(camara.y) + "|" + this->serializarCapas() + "#";
	return estadoInicial;
}

void Servidor::iniciarCamara(){
	camara = {camara.x,camara.y,atoi(handshake->getAncho().c_str()),atoi(handshake->getAlto().c_str())};
}

int Servidor::getCantJugadoresConectadosMax()
{
	return atoi(this->handshake->getCantidadMaximaJugadores().c_str());
}

void Servidor::verificarDesconexion(string nombre)
{
	cout << "Entra a verificar desconexion del jugador" << nombre << endl;
	for (int i = 0; i < jugadores->size(); i++)
	{
		Jugador* jugador = jugadores->at(i);
		if (jugador->getNombre() == nombre)
		{
			jugador->setDesconectado();
			jugador->setSprite("Jugador_desconectado");
			Mensaje* mensaje;
			string mensajeDesconectado = "";
			mensajeDesconectado = jugador->getStringJugador();
			mensaje = new Mensaje(jugador->getNombre(),"Todos",mensajeDesconectado);
			encolarMensajeProcesadoParaCadaCliente(*mensaje,mensajeDesconectado);
			while (this->escuchando && !jugador->getConectado())
			{	usleep(100);
				if (jugador->getPosicion().first < camara.x)
				{
					jugador->setPosicion(camara.x);
					mensajeDesconectado = jugador->getStringJugador();
					mensaje = new Mensaje(jugador->getNombre(),"Todos",mensajeDesconectado);
					encolarMensajeProcesadoParaCadaCliente(*mensaje,mensajeDesconectado);
				}
			}
			jugador->setSprite("Jugador_" + jugador->getEquipo());
			mensajeDesconectado = jugador->getStringJugador();
			mensaje = new Mensaje(jugador->getNombre(),"Todos",mensajeDesconectado);
			encolarMensajeProcesadoParaCadaCliente(*mensaje,mensajeDesconectado);
		}
	}
}
/*
void Servidor::iniciarThreadMovimientoJugador(string nombre){
	pthread_mutex_lock(&mutexVectorJugadores);
	Jugador* jugador = obtenerJugador(nombre);
	pthread_mutex_unlock(&mutexVectorJugadores);
<<<<<<< HEAD
	Servidor* servidor = this;
	ParametrosMovimiento * parametros = new ParametrosMovimiento(servidor,jugador);/*ParametrosActPosicion parametros;
=======
	ParametrosActPosicion parametros;
>>>>>>> 7141cadd9db71637618ff321e8c9e0314344fbbd
	parametros.jugador = jugador;
	parametros.servidor = this;

	pthread_t threadMov = jugador->getThreadMovimiento();
	pthread_create(&threadMov, NULL, &actualizarPosicionesJugador, parametros);
	pthread_detach(threadMov);
	jugador->setThreadMovimiento(threadMov);
<<<<<<< HEAD

}*/



