#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080

int main(int argc, char const *argv[]) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char *filename = "arquivo.txt"; // Substitua pelo seu arquivo
    
    // Abrindo arquivo
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erro ao abrir arquivo");
        exit(EXIT_FAILURE);
    }
    
    // Criando socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Erro ao criar socket \n");
        return -1;
    }
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    // Convertendo endereço IP
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nEndereço inválido/ não suportado \n");
        return -1;
    }
    
    // Conectando ao servidor
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConexão falhou \n");
        return -1;
    }
    
    // Lendo e enviando arquivo
    char buffer[1024] = {0};
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        send(sock, buffer, bytes_read, 0);
        memset(buffer, 0, sizeof(buffer));
    }
    
    fclose(file);
    close(sock);
    return 0;
}