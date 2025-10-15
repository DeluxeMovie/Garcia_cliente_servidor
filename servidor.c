/******************************************
 * Fecha: 14/10/2025
 * Autor: Ángel Daniel García Santana
 * Materia: Sistemas Operativos
 * Tema: Name pipe y FIFO
********************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

// Definición del archivo FIFO
// Se crea en /tmp/ que es un directorio temporal del sistema
#define FIFO_FILE "/tmp/fifo_twoway"

/**
 * Función: reverse_string
 * Propósito: Invertir el orden de los caracteres en una cadena
 * Parámetros:
 *   - str: Puntero a la cadena que se va a invertir
 * Retorno: Ninguno (modifica la cadena original)
 * 
 * Ejemplo: "hola" se convierte en "aloh"
 */
void reverse_string(char *str) {
   int last, limit, first;
   char temp;
   
   // Calcular la última posición de la cadena
   last = strlen(str) - 1;
   // Calcular hasta dónde debemos llegar en el intercambio
   limit = last/2;
   first = 0;
   
   // Intercambiar caracteres desde los extremos hacia el centro
   while (first < last) {
      temp = str[first];
      str[first] = str[last];
      str[last] = temp;
      first++;
      last--;
   }
   return;
}

/**
 * Función: main
 * Propósito: Servidor que procesa mensajes de clientes invirtiendo las cadenas
 * Flujo:
 *   1. Crear el FIFO si no existe
 *   2. Esperar mensajes de clientes
 *   3. Para cada mensaje: invertirlo y enviar respuesta
 *   4. Terminar cuando reciba "end"
 */
int main() {
   int fd;                    // Descriptor del archivo FIFO
   char readbuf[80];          // Buffer para almacenar mensajes recibidos
   char end[10];              // Cadena para comparar con "end"
   int to_end;                // Resultado de comparación con "end"
   int read_bytes;            // Número de bytes leídos
   
   /* Crear el FIFO si no existe 
    * S_IFIFO|0640: 
    *   - S_IFIFO: Indica que es un FIFO
    *   - 0640: Permisos (rw- r-- ---) - dueño:lect/esc, grupo:lect, otros:ninguno
    */
   if (mkfifo(FIFO_FILE, 0640) == -1) {
       // Si falla, probablemente el FIFO ya existe
       perror("mkfifo");
   }
   
   // Preparar la cadena de comparación para finalizar
   strcpy(end, "end");
   
   // Abrir el FIFO para lectura y escritura
   fd = open(FIFO_FILE, O_RDWR);
   if (fd == -1) {
       perror("Error al abrir el FIFO");
       exit(1);
   }
   
   printf("FIFOSERVER: Waiting for messages...\n");
   
   // Bucle principal del servidor
   while(1) {
      // Leer mensaje del cliente (espera hasta que haya datos)
      read_bytes = read(fd, readbuf, sizeof(readbuf));
      readbuf[read_bytes] = '\0';  // Terminar la cadena correctamente
      
      printf("FIFOSERVER: Received string: \"%s\" and length is %d\n", 
             readbuf, (int)strlen(readbuf));
      
      // Verificar si es el comando para terminar
      to_end = strcmp(readbuf, end);
      
      if (to_end == 0) {
         printf("FIFOSERVER: Received end command. Shutting down...\n");
         close(fd);
         // Eliminar el archivo FIFO del sistema
         unlink(FIFO_FILE);
         break;
      }
      
      // Procesar el mensaje: invertir la cadena
      reverse_string(readbuf);
      printf("FIFOSERVER: Sending Reversed String: \"%s\" and length is %d\n", 
             readbuf, (int) strlen(readbuf));
      
      // Enviar respuesta al cliente
      write(fd, readbuf, strlen(readbuf));
      
      /* Pausa para sincronización:
       * Garantiza que el cliente tenga tiempo de leer la respuesta
       * antes de que el servidor lea el siguiente mensaje
       */
      sleep(1);
   }
   return 0;
}
