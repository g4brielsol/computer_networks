/*
author = "Gabriel Sol da Silva"
version = "0.1"
maintainer = "Gabriel Sol da Silva"
status = "Producao"
*/
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
    // quantidade de caracteres
    int caracters = 4095;
    // inicializacao das variaveis
    int socket_file_description, port_number, communication;
    char buffer[caracters];
    // struct que descreve sockets de endereco da internet
    struct sockaddr_in server_address;
    // struct para armazenar informacoes do host
    struct hostent *server;
    // erro se digitar a porta errada
    if (total_parameters < 3)
    {
        fprintf(stderr, "Usou %s como porta\n", parameters[0]);
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
    // Limpa o endereco do server
    bzero((char *)&server_address, sizeof(server_address));
    // protocolo TCP/IP
    server_address.sin_family = AF_INET;
    // copia os bytes da struct hostent para o server address da lista
    bcopy((char *)server->h_addr_list[0], (char *)&server_address.sin_addr.s_addr, server->h_length);
    // htons retorna o valor da ordem do byte da TCP/IP
    server_address.sin_port = htons(port_number);
    // mensagem de erro para a conexao
    if (connect(socket_file_description, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        error("Conexao Falhou");
    }
    // loop para o chat
    while (1)
    {
        // limpa o buffer
        bzero(buffer, 4095);
        // pega o que foi escrito pelo cliente
        fgets(buffer, caracters, stdin);
        // escrita do cliente
        communication = write(socket_file_description, buffer, strlen(buffer));
        if (communication < 0)
        {
            error("Erro na Escrita");
        }
        // ao digitar /quit fecha o loop do while, a funcao foi repetida e nao
        // generalizada porque o "break" so pode ser utilizado em loops
        int quit = strncmp("/quit", buffer, 5);
        if (quit == 0)
        {
            break;
        }
        // limpar o buffer
        bzero(buffer, caracters);
        // leitura do que foi escrito pelo server
        communication = read(socket_file_description, buffer, caracters);
        // mensagem de erro
        if (communication < 0)
        {
            error("Erro na Leitura");
        }
        // printa o que foi escrito pelo Server
        printf("Server: %s", buffer);
        // ao digitar /quit fecha o loop do while, a funcao foi repetida e nao
        // generalizada porque o "break" so pode ser utilizado em loops
        quit = strncmp("/quit", buffer, 5);
        if (quit == 0)
        {
            break;
        }
    }
    // fecha o socket
    close(socket_file_description);
    return 0;
}
