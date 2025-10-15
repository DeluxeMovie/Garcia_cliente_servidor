/******************************************
 * Fecha: 14/10/2025
 * Autor: Ángel Daniel García Santana
 * Materia: Sistemas Operativos
 * Tema: Namepipe y FIFO
********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

// Misma definición del FIFO que en el servidor
// Ambos programas deben usar la misma ruta para comunicarse
#define FIFO_FILE "/tmp/fifo_twoway"

/**
 * Función: main
 * Propósito: Cliente que envía mensajes al servidor y muestra respuestas
 * Flujo:
 *   1. Conectar al FIFO del servidor
 *   2. Leer entrada del usuario
 *   3. Enviar al servidor y esperar respuesta
 *   4. Mostrar resultado
 *   5. Repetir hasta que usuario escriba "end"
 */
int main() {
   int fd;                    // Descriptor del archivo FIFO
   int end_process;           // Flag para controlar fin del programa
   int stringlen;             // Longitud de la cadena ingresada
   int read_bytes;            // Bytes leídos del servidor
   char readbuf[80];          // Buffer para entrada y respuestas
   char end_str[5];           // Cadena de comparación "end"
   
   printf("FIFO_CLIENT: Send messages, infinitely, to end enter \"end\"\n");
   
   // Abrir el FIFO (debe existir, creado por el servidor)
   fd = open(FIFO_FILE, O_RDWR);
   if (fd == -1) {
       perror("Error al abrir el FIFO");
       exit(1);
   }
   
   // Preparar cadena para detectar comando de salida
   strcpy(end_str, "end");
   
   // Bucle principal del cliente
   while (1) {
      printf("Enter string: ");
      // Leer entrada del usuario de forma segura
      fgets(readbuf, sizeof(readbuf), stdin);
      
      // Calcular longitud y eliminar el salto de línea
      stringlen = strlen(readbuf);
      readbuf[stringlen - 1] = '\0';
      
      // Verificar si el usuario quiere terminar
      end_process = strcmp(readbuf, end_str);
      
      if (end_process != 0) {
         // Enviar mensaje normal al servidor
         write(fd, readbuf, strlen(readbuf));
         printf("FIFOCLIENT: Sent string: \"%s\" and string length is %d\n", 
                readbuf, (int)strlen(readbuf));
         
         // Esperar y leer respuesta del servidor
         read_bytes = read(fd, readbuf, sizeof(readbuf));
         readbuf[read_bytes] = '\0';  // Asegurar terminación de cadena
         printf("FIFOCLIENT: Received string: \"%s\" and length is %d\n", 
                readbuf, (int)strlen(readbuf));
      } else {
         // Enviar comando de finalización
         write(fd, readbuf, strlen(readbuf));
         printf("FIFOCLIENT: Sent string: \"%s\" and string length is %d\n", 
                readbuf, (int)strlen(readbuf));
         close(fd);
         break;
      }
   }
   return 0;
}
