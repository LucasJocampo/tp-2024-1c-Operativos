#ifndef SERVER_H_
#define SERVER_H_

#include"estructurasConexion.h"
#include <commons/log.h>
#include<assert.h>
#include "handshake.h"

#define MAXCONN 5

extern t_log* loggerServ;

int iniciar_servidor(char* puerto, t_log* logger);
int esperar_cliente(int socket_servidor, modulo_code modulo);

#endif /* SERVER_H_ */
