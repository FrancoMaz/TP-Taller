
OBJSCLI = MainCliente.cpp Cliente.cpp Mensaje.cpp Logger.cpp VentanaSDL.cpp TexturaSDL.cpp Vista.cpp Controlador.cpp SetDeSpritesDto.cpp SpriteDto.cpp ImagenDto.cpp

OBJSSERV = MainServidor.cpp Servidor.cpp Mensaje.cpp Logger.cpp XmlParser.cpp SetDeSpritesDto.cpp SpriteDto.cpp ImagenDto.cpp  pugixml.hpp pugixml.cpp pugiconfig.hpp

CC = g++ 

COMPILER_FLAGS = -w 
 
LINKER_FLAGS_CLIENT = -std=c++11 -lpthread -lSDL2 -lSDL2_image -lSDL2_ttf
LINKER_FLAGS_SERVER = -std=c++11 -lpthread

OBJCLI_NAME = cliente
OBJSERV_NAME = servidor

cliente: $(OBJSCLI)
	$(CC) $(OBJSCLI) $(COMPILER_FLAGS) $(LINKER_FLAGS_CLIENT) -o $(OBJCLI_NAME)

servidor: $(OBJSERV)
	$(CC) $(OBJSSERV) $(COMPILER_FLAGS) $(LINKER_FLAGS_SERVER) -o $(OBJSERV_NAME)

clean:
	rm -rf *.o $(OBJCLI_NAME) $(OBJSERV_NAME)


