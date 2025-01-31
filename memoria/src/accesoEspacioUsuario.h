#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include <commons/config.h>
#include <utils/server.h>
#include <utils/serializacion.h>
#include <pthread.h>
#include <semaphore.h>
#include "conexiones.h"
#include "estructuras.h"
#include "memoriaCPU.h"
#include "memoriaKernel.h"
#include "memoriaDeInstrucciones.h"
#include "memoriaContigua.h"
#include "main.h"

extern pthread_mutex_t mutex_memoria_contigua;
void escribir_memoria(int socket);
void leer_memoria(int socket);