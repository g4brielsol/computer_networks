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
    if (total_parameters < 2)
    {
        // mensagem de erro
        fprintf(stderr, "Numero da porta indeterminado. Fim do Programa\n");
        // termina o programa
        exit(1);
    }

    int socket_file_description, new_socket_file_description, port_number, communication;
    char buffer[16];

    // struct que descreve sockets de endereco da internet
    struct sockaddr_in server_address, client_address;
    //  tamanho do socket, retorna os valores e membros
    socklen_t client_length;

    socket_file_description = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_file_description < 0)
    {
        error("Erro ao abrir o Socket.\n");
    }

    // Limpa o endereco do server
    bzero((char *)&server_address, sizeof(server_address));
    // transformar string em inteiro do parametro passado pelo cliente -> porta do server
    port_number = atoi(parameters[1]);

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    // htons = host the network shut
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

    // loop para manter o chat aberto
    while (1)
    {
        // Limpa a mensagem do buffer
        bzero(buffer, 16);
        // leitura da mensagem do cliente
        communication = read(new_socket_file_description, buffer, 16);
        // mensagem caso haja um erro de leitura
        if (communication < 0)
        {
            error("Erro ao Ler");
        }
        printf("Client: %s", buffer);
        // Limpa a mensagem do buffer
        bzero(buffer, 16);
        // ler mensagem escrita pelo servidor
        fgets(buffer, 16, stdin);
        // escrita da mensagem do server
        communication = write(new_socket_file_description, buffer, strlen(buffer)); //strlen(buffer) or 16
        // mensagem caso haja um erro na escrita
        if (communication < 0)
        {
            error("Erro ao Escrever");
        }

        int quit = strncmp("quit", buffer, 4);
        if (quit == 0)
        {
            break;
        }
    }
    close(new_socket_file_description);
    close(socket_file_description);
    return 0;
}
