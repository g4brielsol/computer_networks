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

// funcao de erro
void error(const char *message)
{
    // a funcao perror interpreta o numero do erro e possui um output padrao
    perror(message);
    // termina o programa
    exit(1);
}

int main(int total_parameters, char *parameters[])
{
    // quantidade de caracteres
    int caracters = 4095;
    // caso a porta do server nao seja passada
    if (total_parameters < 2)
    {
        // mensagem de erro
        fprintf(stderr, "Numero da porta indeterminado. Fim do Programa\n");
        // termina o programa
        exit(1);
    }
    // inicializacao das variaveis
    int socket_file_description, new_socket_file_description, port_number, communication;
    char buffer[caracters];

    // struct que descreve sockets de endereco da internet
    struct sockaddr_in server_address, client_address;
    //  tamanho do socket, retorna os valores e membros
    socklen_t client_length;
    // funcao para o SO entender o arquivo descritor para um socket que pode ser usado para uma conexao de rede
    socket_file_description = socket(AF_INET, SOCK_STREAM, 0);
    // mensagem de erro
    if (socket_file_description < 0)
    {
        error("Erro ao abrir o Socket.\n");
    }
    // reusar porta para nao haver erro de compilacao
    if (setsockopt(socket_file_description, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0)
    {
        error("setsockopt(SO_REUSEADDR) failed");
    }
    // Limpa o endereco do server
    bzero((char *)&server_address, sizeof(server_address));
    // transformar string em inteiro do parametro passado pelo cliente -> porta do server
    port_number = atoi(parameters[1]);
    // protocolo TCP/IP
    server_address.sin_family = AF_INET;
    // endereco de IP nao especificado
    server_address.sin_addr.s_addr = INADDR_ANY;
    // htons retorna o valor da ordem do byte da TCP/IP
    server_address.sin_port = htons(port_number);
    // mensagen de erro caso a conexao nao seja feita
    if (bind(socket_file_description, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        error("Falha na Conexao.");
    }

    //listenner para quem quer conectar ao socket, o numero significa o limite de clientes possiveis, nesse caso 1
    listen(socket_file_description, 1);
    client_length = sizeof(client_address);
    // conexao do cliente
    new_socket_file_description = accept(socket_file_description, (struct sockaddr *)&client_address, &client_length);
    // mensagem de erro caso nao seja possivel aceitar o cliente
    if (new_socket_file_description < 0)
    {
        error("Erro ao Aceitar Cliente");
    }
    // loop para manter o socket
    while (1)
    {
        // Limpa a mensagem do buffer
        bzero(buffer, caracters);
        // leitura da mensagem do cliente
        communication = read(new_socket_file_description, buffer, caracters);
        int size = strlen(buffer);
        // mensagem caso haja um erro de leitura
        if (communication < 0)
        {
            error("Erro ao Ler");
        }
        else if (size <= caracters)
        {
            // printa a mensagem do cliente
            printf("Client: %s", buffer);
        }
        // ao digitar /quit fecha o loop do while, a funcao foi repetida e nao
        // generalizada porque o "break" so pode ser utilizado em loops
        int quit = strncmp("/quit", buffer, 5);
        if (quit == 0)
        {
            break;
        }
        // Limpa a mensagem do buffer
        bzero(buffer, caracters);
        // ler mensagem escrita pelo servidor
        fgets(buffer, caracters, stdin);
        // escrita da mensagem do server
        communication = write(new_socket_file_description, buffer, strlen(buffer)); //strlen(buffer) or caracters
        // mensagem caso haja um erro na escrita
        if (communication < 0)
        {
            error("Erro ao Escrever");
        }
        // ao digitar /quit fecha o loop do while, a funcao foi repetida e nao
        // generalizada porque o "break" so pode ser utilizado em loops
        quit = strncmp("/quit", buffer, 5);
        if (quit == 0)
        {
            break;
        }
    }
    // fecha os sockets
    close(new_socket_file_description);
    close(socket_file_description);
    return 0;
}
