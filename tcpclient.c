#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>

#define buffer 1024


void getUserName(char* userName)
{
    do
    {
        printf("Enter a user name of up to 10 characters: ");
        scanf("%s", userName);
    } while(strlen(userName) > 10);
}

void exchangeUserNames(char* userName, char* serverName, int sockFD)
{
    int result;
    result = send(sockFD, userName, strlen(userName), 0);
    result = recv(sockFD, serverName, buffer, 0);
}

void sendMsg(int sockFD, char* userName)
{
    int result;
    char outbound[buffer];
    memset(outbound, 0, sizeof(outbound));
    printf("%s> ", userName);
    fgets(outbound, buffer, stdin);
    result = send(sockFD, outbound, buffer, 0);
    if(strcmp(outbound, "\\quit\n") == 0)
    {
        printf("Exiting client...");
        exit(0);
    }
}

void recvMsg(int sockFD, char* serverName)
{
    int result;
    char inbound[buffer];
    memset(inbound, 0, sizeof(inbound));
    result = recv(sockFD, inbound, buffer, 0);
    if(strcmp(inbound, "\\quit") == 0)
    {
        printf("Connection closed by server, closing client...\n");
        exit(0);
    }
    
    printf("%s> %s", serverName, inbound);
}

void chat(int sockFD, char* userName, char* serverName)
{
    while(1)
    {
        sendMsg(sockFD, userName);
        recvMsg(sockFD, serverName);
    }
}

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        fprintf(stderr, "USAGE: %s hostname port\n", argv[0]);
        exit(1);
    }
    
    int sockFD, portNumber, result;
    struct addrinfo hints;
    struct addrinfo* res = 0;
    char userName[15];
    char serverName[15];
    memset(userName, 0, sizeof(userName));
    memset(serverName, 0, sizeof(serverName));
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    
    result = getaddrinfo(argv[1], argv[2], &hints, &res);
    if(result != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(result));
        exit(1);
    }
    
    sockFD = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    result = connect(sockFD, res->ai_addr, res->ai_addrlen);
    freeaddrinfo(res);
    getUserName(userName);
    exchangeUserNames(userName, serverName, sockFD);
    chat(sockFD, userName, serverName);
    
    return 0;
}

