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

 char userName[15];
 char serverName[15];

//Simply retrieves the user name for the client from the user
void getUserName()
{
    do
    {
        printf("Enter a user name of up to 10 characters: ");
        fgets(userName, 15, stdin);
    } while(strlen(userName) > 10);
	userName[strcspn(userName, "\n")] = 0;
}

//This sends the client's user name to the server and then waits to get the server's user name back
void exchangeUserNames(int sockFD)
{
    int result;	
    result = send(sockFD, userName, strlen(userName), 0);
    result = recv(sockFD, serverName, buffer, 0);
	if(result < 0)
	{
		printf("ERROR\n");
	}
}

//This sends a message to the server.  It gets the client's message input and strips the newline and then sends.
//It checks for an input of "\quit" and exits if found.
void sendMsg(int sockFD)
{
    int result;
    char outbound[buffer];
    memset(outbound, 0, sizeof(outbound));
    printf("%s> ", userName);
    fgets(outbound, buffer, stdin);
	outbound[strcspn(outbound, "\n")] = 0;
    result = send(sockFD, outbound, buffer, 0);
    if(strcmp(outbound, "\\quit") == 0)
    {
        printf("Exiting client...\n");
        exit(0);
    }
}

//This receives a message from the server.  If the message is "\quit", the program exits".  Otherwise, it prints
//the received message.
void recvMsg(int sockFD)
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
    
    printf("%s> %s\n", serverName, inbound);
}

//This simply loops endlessly, sending and then receiving messages, until "\quit" is entered
void chat(int sockFD)
{
    while(1)
    {
        sendMsg(sockFD);
        recvMsg(sockFD);
    }
}

//The main routine establishes the connection, gets a user name, exchanges user names with the server, and then begins the chat loop.
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
	if (sockFD == -1)
	{
		perror("Socket error!\n");
	}
	
    result = connect(sockFD, res->ai_addr, res->ai_addrlen);
	if (result == -1)
	{
		perror("Error connecting!\n");
	}
	
    freeaddrinfo(res);
    getUserName(userName);
    exchangeUserNames(sockFD);
	chat(sockFD);
    
    return 0;
}

