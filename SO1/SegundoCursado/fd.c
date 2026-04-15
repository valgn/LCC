#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>   // Necesario para open() y las flags (O_RDONLY, etc.)
#include <unistd.h>  // Necesario para read(), write(), close()

int main() {
    // 1. Reservamos nuestro buffer de 1024 bytes.
    // Le sumamos 1 byte extra si planeamos imprimirlo como un string (para el '\0').
    char buffer[1025]; 
    ssize_t bytes_leidos;

    // 2. Abrimos el archivo en modo "Solo Lectura" (O_RDONLY)
    int fd = open("texto.txt", O_RDONLY);

    // Siempre hay que verificar si open() falló (devuelve -1)
    if (fd == -1) {
        perror("Error al abrir el archivo (¿creaste 'texto.txt'?)");
        return EXIT_FAILURE;
    }

    printf("¡Archivo abierto! El File Descriptor asignado es: %d\n", fd);

    // 3. Leemos hasta 1024 bytes del File Descriptor
    bytes_leidos = read(fd, buffer, 1024);

    // Verificamos si read() falló (devuelve -1)
    if (bytes_leidos == -1) {
        perror("Error al leer el archivo");
        close(fd); // Buenas prácticas: cerrar antes de salir
        return EXIT_FAILURE;
    }

    // Como read() lee bytes crudos y no sabe de "strings", 
    // le agregamos el terminador nulo manualmente al final para poder usar printf
    buffer[bytes_leidos] = '\0';

    // 4. Mostramos los resultados
    printf("Se intentaron leer 1024 bytes. Se leyeron realmente: %zd bytes.\n", bytes_leidos);
    printf("Contenido:\n---\n%s\n---\n", buffer);

    // 5. Cerramos el File Descriptor para liberar el recurso en el SO
    if (close(fd) == -1) {
        perror("Error al cerrar el archivo");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}