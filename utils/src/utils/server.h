#ifndef SERVER_H_
#define SERVER_H_

#include"estructurasConexion.h"
#include<assert.h>
#include "handshake.h"

#define MAXCONN 5

extern t_log* logger;

void* recibir_buffer(int*, int);

int iniciar_servidor(char*);
void esperar_cliente(int,t_conexion_escucha*);
t_list* recibir_paquete(int);
void recibir_mensaje(int);
int recibir_operacion(int);
void* atender_cliente(int*);

void iterator(char* value);
int escucharConexiones(t_conexion_escucha*);

#endif /* SERVER_H_ */
