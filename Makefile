# Compilador
CC = gcc

# Flags de compilación
CFLAGS = -Wall -Wextra -std=c99 -pedantic

# Nombres de los ejecutables
CLIENTE = cliente
SERVIDOR = servidor

# Archivos fuente
CLIENTE_SRC = cliente.c
SERVIDOR_SRC = servidor.c

# Regla por defecto (se ejecuta con solo 'make')
all: $(CLIENTE) $(SERVIDOR)

# Compilar el cliente
$(CLIENTE): $(CLIENTE_SRC)
	$(CC) $(CFLAGS) -o $(CLIENTE) $(CLIENTE_SRC)
	@echo "Cliente compilado exitosamente"

# Compilar el servidor
$(SERVIDOR): $(SERVIDOR_SRC)
	$(CC) $(CFLAGS) -o $(SERVIDOR) $(SERVIDOR_SRC)
	@echo "Servidor compilado exitosamente"

# Regla para compilar solo el cliente
cliente: $(CLIENTE)

# Regla para compilar solo el servidor
servidor: $(SERVIDOR)

# Limpiar archivos compilados
clean:
	rm -f $(CLIENTE) $(SERVIDOR)
	@echo "Archivos ejecutables eliminados"

# Limpiar y recompilar todo
rebuild: clean all

# Ayuda
help:
	@echo "Opciones disponibles:"
	@echo "  make all      - Compila cliente y servidor"
	@echo "  make cliente  - Compila solo el cliente"
	@echo "  make servidor - Compila solo el servidor"
	@echo "  make clean    - Elimina los ejecutables"
	@echo "  make rebuild  - Limpia y recompila todo"
	@echo "  make help     - Muestra esta ayuda"

# Especificar que estas reglas no son archivos
.PHONY: all clean rebuild help cliente servidor
