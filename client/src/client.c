#include "client.h"

int main(void) {
    /*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

    int conexion;
    char *ip;
    char *puerto;
    char *valor;

    t_config *config;

    /* ---------------- LOGGING ---------------- */

    // iniciar_logger();
	logger = log_create("tp0.log", "tp0", true, LOG_LEVEL_INFO);

    // Usando el logger creado previamente
    // Escribi: "Hola! Soy un log"

    log_info(logger, "Hola! Soy un log");

    /* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

    config = iniciar_config();

    // Usando el config creado previamente, leemos los valores del config y los
    // dejamos en las variables 'ip', 'puerto' y 'valor'

    // Loggeamos el valor de config
    valor = config_get_string_value(config, "CLAVE");
    log_info(logger, "El valor de CLAVE es: %s", valor);

    /* ---------------- LEER DE CONSOLA ---------------- */

    leer_consola(logger);

    /*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

    // ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
    // Creamos una conexión hacia el servidor
    conexion = crear_conexion(ip, puerto);

    // Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor, conexion);

    // Armamos y enviamos el paquete
    paquete(conexion);

    terminar_programa(conexion, logger, config);

    /*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
    // Proximamente
}

// t_log *iniciar_logger(void) {
//     logger = log_create("tp0.log", "tp0", true, LOG_LEVEL_INFO);

//     return logger;
// }

t_config *iniciar_config(void) {
    char *config_file = "cliente.config";
    t_config *nuevo_config = config_create(config_file);

    if (nuevo_config == NULL) {
        log_error(logger, "Error obteniendo las configuraciones de %s", config_file);
        abort();
    }

    return nuevo_config;
}

void leer_consola(t_log *logger) {
    char *leido;

    // La primera te la dejo de yapa
    leido = readline("> ");

    // El resto, las vamos leyendo y logueando hasta recibir un string vacío
    while (strcmp(leido, "") != 0) {
        log_info(logger, "Leido: %s", leido);
        free(leido);
        leido = readline("> ");
    }

    free(leido);
}

void paquete(int conexion) {
    // Ahora toca lo divertido!
    char *leido;
    t_paquete *paquete;

	paquete = crear_paquete();
	leido = readline("> ");

    // El resto, las vamos leyendo y logueando hasta recibir un string vacío
    while (strcmp(leido, "") != 0) {
		agregar_a_paquete(paquete, leido, strlen(leido)+1);
		log_info(logger, "Leido: %s", leido);
        free(leido);
        leido = readline("> ");
    }

    free(leido);

	enviar_paquete(paquete, conexion);
	eliminar_paquete(paquete);

    // Leemos y esta vez agregamos las lineas al paquete

    // ¡No te olvides de liberar las líneas y el paquete antes de regresar!
}

void terminar_programa(int conexion, t_log *logger, t_config *config) {
    /* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config)
      con las funciones de las commons y del TP mencionadas en el enunciado */
    log_destroy(logger);
    config_destroy(config);
	liberar_conexion(conexion);
}
