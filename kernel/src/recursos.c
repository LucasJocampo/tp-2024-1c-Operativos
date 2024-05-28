#include <recursos.h>

t_list* listaRecursos;

void leerRecursosDeConfig(t_config* config) {
    char** recursos = config_get_array_value(config, "RECURSOS");
    char** instancias = config_get_array_value(config, "INSTANCIAS_RECURSOS");

    listaRecursos = list_create();

    int size = string_array_size(recursos);

    for (int i = 0; i < size; i++) {
        t_recurso* recurso = malloc(sizeof(t_recurso));

        recurso->nombre = recursos[i];
        recurso->instancias = atoi(instancias[i]);
        recurso->procesosBloqueados = queue_create();

        list_add(listaRecursos, recurso);
    }
}