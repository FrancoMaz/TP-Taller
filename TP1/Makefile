
OBJSCLI = MainCliente.cpp Cliente.cpp Mensaje.cpp 

OBJSSERV = MainServidor.cpp Servidor.cpp Mensaje.cpp Logger.cpp

CC = g++ 

COMPILER_FLAGS = -w 
 
LINKER_FLAGS = -lpthread

OBJCLI_NAME = cliente
OBJSERV_NAME = servidor

client: $(OBJSCLI)
	$(CC) $(OBJSCLI) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJCLI_NAME)

servidor: $(OBJSERV)
	$(CC) $(OBJSSERV) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJSERV_NAME)

clean:
	rm -rf *.o $(OBJCLI_NAME) $(OBJSERV_NAME)


