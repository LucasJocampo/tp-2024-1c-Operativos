#include <utils/serializacion.h>
#include <stdlib.h>
#include <string.h>
#include <commons/collections/list.h>
#include "estructuras.h"
#include "memoriaDeInstrucciones.h"
#include "main.h"

t_list* lista_de_procesos_con_ins = NULL;
t_list* tablas_de_paginas = NULL;
pthread_mutex_t mutex_tablas_paginas = PTHREAD_MUTEX_INITIALIZER;

proceso_memoria* creacion_proceso(int socket_kernel) {
    op_code cod_op = recibir_operacion(socket_kernel);
    if(cod_op == CREACION_PROCESO){
        proceso_memoria* proceso = malloc(sizeof(proceso_memoria));
        t_buffer* buffer = recibir_buffer(socket_kernel);
        int pid_proceso= buffer_read_int(buffer);
        proceso->proceso_id=pid_proceso;
        char* path_proceso = buffer_read_string(buffer);
        proceso->path= path_proceso;
        proceso->info_de_tabla = NULL; // info arranca como lista vacía ya q arranca todo vacío
        liberar_buffer(buffer);
        return proceso;
    }
    return NULL;
}

void abrir_archivo_path(int socket_kernel){
    proceso_memoria* proceso = creacion_proceso(socket_kernel);
    char* path = (proceso->path);
    if (path == NULL){
        exit(1);
    }
    FILE *file = fopen(path,"r");
    if (file == NULL){
            fprintf(stderr, "Archivo vacio");
            free(path);
        }
    char* linea = NULL;
    size_t tamanio = 0;
    ssize_t leidos;
    t_list* instrucciones;
    instrucciones = list_create();
    t_proceso* proceso_ins = malloc(sizeof(t_proceso));
    proceso_ins->pid= proceso->proceso_id;

    while ((leidos = getline(&linea, &tamanio,file)) != -1){
        if(linea[leidos - 1] == '\n'){
            linea[leidos - 1] = '\0';
        }

        char *linea_copia = strdup(linea);
        if (linea_copia == NULL){
            perror("Error al copiar linea");
            free(linea);
            fclose(file);
            return;
        }
        list_add(instrucciones, linea_copia);
    }
    proceso_ins->instrucciones = instrucciones;
    list_add(lista_de_procesos_con_ins,proceso_ins); // guardo en la lista de los procesos el proceso!

    t_paquete* paquete = crear_paquete(PAQUETE);
    enviar_paquete(paquete, socket_kernel);

    free(linea);
    fclose(file);
}

proceso_memoria* hallar_proceso_tabla_pags(int PID){
    bool _mismoPID(proceso_memoria* proceso){
        return (proceso->pid == PID);
    };

    pthread_mutex_lock(&mutex_tablas_paginas);
    t_proceso* encontrado = list_find(tablas_de_paginas, (void *)_mismoPID);
    pthread_mutex_unlock(&mutex_tablas_paginas);

    return encontrado;
}

void destruir_proceso(proceso_memoria* proceso){
    if(proceso!=NULL){
        list_destroy_and_destroy_elements(proceso->instrucciones,free); // libera memoria asignada a cada instruccion del proceso y la lista proceso->instrucciones
        free(proceso); //libera la memoria asignada al propio proceso
    }
}

void eliminar_proceso_tabla_pags(int PID){
    bool _mismoPID(proceso_memoria* proceso){
        return (proceso->pid == PID);
    };
    pthread_mutex_lock(&mutex_tablas_paginas);
    list_remove_and_destroy_by_condition(tablas_de_paginas, (void*)_mismoPID, (void*) destruir_proceso);
    pthread_mutex_unlock(&mutex_tablas_paginas);
    
}

void finalizar_proceso(int socket_kernel){
    op_code cod_op = recibir_operacion(socket_kernel);
    if(cod_op == FIN_PROCESO){
        t_buffer* buffer = recibir_buffer(socket_kernel);
        int pid_proceso= buffer_read_int(buffer);
        eliminar_proceso_de_lista_ins(pid_proceso);
        eliminar_proceso_tabla_pags(pid_proceso);
        liberar_buffer(buffer);
    }

}
