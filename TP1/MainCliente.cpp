/*
 * mainCliente.cpp
 *
 *  Created on: Aug 30, 2016
 *      Author: franco
 */

#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include "Cliente.h"

using namespace std;

bool chequearSocket(string ip, int puerto)
{
	return true;
}

void* cicloConexion(void* arg)
{
	Cliente cliente = *(Cliente*)arg;
	while(cliente.getOpcionMenu() != 5 and cliente.getOpcionMenu() != 4) //mientras la opcion del menu no sea salir o desconectar..
	{
		cliente.mostrarMenuYProcesarOpcion();
	}
	return NULL;
}

int main()
{
	char* ip;
	int puerto, accion;
	bool socketOk = false;
	while(!socketOk)
	{
		cout << "Ingrese el puerto para la conexion: ";
		cin >> puerto;
		cout << "Ingrese la ip del servidor:";
		cin >> ip;
		socketOk = chequearSocket(ip, puerto); //FALTA IMPLEMENTAR METODO DE CHEQUEAR IP/PUERTO. ESTA MAS ABAJO LA FUNCION.
		if (!socketOk)
		{
			//SI NO SE PUDO COMPROBAR LA CONEXION, DOY ERROR Y VUELVO A PEDIR IP Y CONEXION
			cout << "Error: La direccion de ip o el puerto no permiten esta conexion." << endl;
		}
	}
	cout << "SOCKET OK" << endl;
	Cliente *cliente = new Cliente(ip, puerto);
	string nombre, contrasenia;
	cout << "SISTEMA DE MENSAJERIA" << endl;
	while (accion < 1 or accion > 2)
	{
		cout << "1) Conectar" << endl;
		cout << "2) Salir" << endl;
		cout << "Que desea hacer? " << endl;
		cin >> accion;
	}
	if (accion == 2)
	{
		//si la accion es salir, devuelvo 0 y salgo del programa.
		return 0;
	}
	//si no es salir, intento conectar pidiendo usuario y contraseña.
	/*while (cliente->getClientesDisponibles().empty())
	{
		cout << "Ingrese nombre de usuario: ";
		cin >> nombre;
		cout << "Ingrese contrasenia: ";
		cin >> contrasenia;
		cliente->setClientesDisponibles(nombre,contrasenia);
		if (cliente->getClientesDisponibles().empty())
		{
			//si despues de intentar conectar, sigue siendo una lista vacia, muestro error y pido nuevamente.
			cout << "Error al intentar autenticar. Ingrese un nombre de usuario y una contrasenia validos." << endl;
		}
	}*/
	string opc = "";
	while (opc == "")
	{
		cout << "hola" << endl;
		cin >> opc;
	}
	//cout << "Autenticación OK. Bienvenido al sistema de mensajería. ¿Qué acción desea realizar?" << endl;
	//cliente->mostrarMenu();
	pthread_t thrComu = cliente->getThreadComunicacion();
	int threadOk = pthread_create(&thrComu,NULL, &cicloConexion,cliente );
	if (threadOk != 0)
	{
		cout << "Error al inicializar la conexion." << endl;
	}
	else{
		cout << "Thread comu ok" << endl;
		pthread_join(cliente->getThreadComunicacion(), NULL);
	}
	return 0;
}























	  /****************************************************************************************/
	 /***************** ACA ABAJO ESTA EL EJEMPLO QUE ESTABA EN INTERNET *********************/
	/****************************************************************************************/


    /* ---------- INITIALIZING VARIABLES ---------- */

    /*
       1. client is a file descriptor to store the values
       returned by the socket system call and the accept
       system call.

       2. portNum is for storing port number on which
       the accepts connections

       3. isExit is bool variable which will be used to
       end the loop

       4. The client reads characters from the socket
       connection into a dynamic variable (buffer).

       5. A sockaddr_in is a structure containing an internet
       address. This structure is already defined in netinet/in.h, so
       we don't need to declare it again.

        DEFINITION:

        struct sockaddr_in
        {
          short   sin_family;
          u_short sin_port;
          struct  in_addr sin_addr;
          char    sin_zero[8];
        };

        6. serv_addr will contain the address of the server

    */

  /*  int client;
    int portNum = 1500; // NOTE that the port number is same for both client and server
    bool isExit = false;
    int bufsize = 1024;
    char buffer[bufsize];
    char* ip = "127.0.0.1";

    struct sockaddr_in server_addr;

    client = socket(AF_INET, SOCK_STREAM, 0);

     ---------- ESTABLISHING SOCKET CONNECTION ----------
     --------------- socket() function ------------------

    if (client < 0)
    {
        cout << "\nError establishing socket..." << endl;
        exit(1);
    }


        The socket() function creates a new socket.
        It takes 3 arguments,

            a. AF_INET: address domain of the socket.
            b. SOCK_STREAM: Type of socket. a stream socket in
            which characters are read in a continuous stream (TCP)
            c. Third is a protocol argument: should always be 0. The
            OS will choose the most appropiate protocol.

            This will return a small integer and is used for all
            references to this socket. If the socket call fails,
            it returns -1.



    cout << "\n=> Socket client has been created..." << endl;


        The variable serv_addr is a structure of sockaddr_in.
        sin_family contains a code for the address family.
        It should always be set to AF_INET.

        htons() converts the port number from host byte order
        to a port number in network byte order.



    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portNum);

    // this function returns returns 1 if the IP is valid
    // and 0 if invalid
    // inet_pton converts IP to packets
    // inet_ntoa converts back packets to IP
    //inet_pton(AF_INET, ip, &server_addr.sin_addr);

    if (connect(client,(struct sockaddr *)&server_addr, sizeof(server_addr)) == 0)
        cout << "=> Connection to the server " << inet_ntoa(server_addr.sin_addr) << " with port number: " << portNum << endl;


     ---------- CONNECTING THE SOCKET ----------
     ---------------- connect() ----------------

    if (connect(client,(struct sockaddr *)&server_addr, sizeof(server_addr)) == 0)
        cout << "=> Connection to the server port number: " << portNum << endl;


        The connect function is called by the client to
        establish a connection to the server. It takes
        three arguments, the socket file descriptor, the
        address of the host to which it wants to connect
        (including the port number), and the size of this
        address.

        This function returns 0 on success and -1
        if it fails.

        Note that the client needs to know the port number of
        the server but not its own port number.


    cout << "=> Awaiting confirmation from the server..." << endl; //line 40
    recv(client, buffer, bufsize, 0);
    cout << "=> Connection confirmed, you are good to go...";

    cout << "\n\n=> Enter # to end the connection\n" << endl;

    // Once it reaches here, the client can send a message first.

    do {
        cout << "Client: ";
        do {
            cin >> buffer;
            send(client, buffer, bufsize, 0);
            if (*buffer == '$') {
                send(client, buffer, bufsize, 0);
                *buffer = '*';
                isExit = true;
            }
        } while (*buffer != 42);
        if (!isExit)
			{
			cout << "Server: ";
			do {
				recv(client, buffer, bufsize, 0);
				cout << buffer << " ";
				if (*buffer == '#') {
					*buffer = '*';
					isExit = true;
				}

			} while (*buffer != 42);
			cout << endl;
        }

    } while (!isExit);

     ---------------- CLOSE CALL -------------
     ----------------- close() ---------------


        Once the server presses # to end the connection,
        the loop will break and it will close the server
        socket connection and the client connection.


    cout << "\n=> Connection terminated.\nGoodbye...\n";

    close(client);
    return 0;
}*/




