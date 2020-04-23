#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

// funcao de erro
void error(const char *message)
{
    // a funcao perror interpreta o numero do erro e possui um output padrao
    perror(message);
    // termina o programa
    exit(0);
}

int main(int total_parameters, char *parameters[])
{
    int socket_file_description, port_number, communication;
    char buffer[16];
    struct sockaddr_in server_address;
    // struct para armazenar informacoes do host
    struct hostent *server;
    // erro se digitar a porta errada
    if (total_parameters < 3)
    {
        fprintf(stderr, "usou %s como porta\n", parameters[0]);
        exit(1);
    }
    // numero da porta
    port_number = atoi(parameters[2]);
    // socket para conectar
    socket_file_description = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_file_description < 0)
    {
        error("Erro ao abrir o socket");
    }
    // porta ip do server
    server = gethostbyname(parameters[1]);
    // mensagem de erro caso nao consiga conectar ao host
    if (server == NULL)
    {
        fprintf(stderr, "Erro, host nao existe");
    }

    bzero((char *)&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    // copia os bytes da struct hostent para o server address da lista
    bcopy((char *)server->h_addr_list[0], (char *)&server_address.sin_addr.s_addr, server->h_length);
    // host para a network
    server_address.sin_port = htons(port_number);
    // mensagem de erro para a conexao
    if (connect(socket_file_description, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        error("Conexao Falhou");
    }

    while (1)
    {
        bzero(buffer, 16);
        fgets(buffer, 16, stdin);
        communication = write(socket_file_description, buffer, strlen(buffer));
        if (communication < 0)
        {
            error("Erro na Escrita");
        }
        bzero(buffer, 16);
        communication = read(socket_file_description, buffer, 16);
        if (communication < 0)
        {
            error("Erro na Leitura");
        }
        printf("Server: %s", buffer);

        int quit = strncmp("quit", buffer, 4);
        if (quit == 0)
        {
            break;
        }
    }

    close(socket_file_description);
    return 0;
}