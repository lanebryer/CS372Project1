#!/usr/bin/env python

from socket import *
import sys

#this block checks for the correct number of arguments
if __name__ == "__main__":
    if len(sys.argv) != 2:
        print 'Please enter a port number to listen on'
        exit(1)
        
port = sys.argv[1]
serverName = "server"


buffer = 1024

#These three lines establish the socket, bind it to the port given by the user, and then listens on that port
s = socket(AF_INET, SOCK_STREAM)
s.bind(('', int(port)))
s.listen(1)

#this exchanges user names with the client
def exchangeNames(conn):
    userName = conn.recv(buffer)
    userName = userName + "> "
    conn.sendall(serverName)
    return userName
   
#this is the basic chat loop which receives and then sends a message in a loop   
def chat(conn):
    while(1):
        quit = recvMsg(conn)        
        if (quit == 1):
            print "Client closed connection..."
            break
        outboundMsg = sendMsg(conn)
        if (outboundMsg == "\quit"):
            print "Closing client...\n"
            break
   
#this sends a message to the client by getting user input and then sending   
def sendMsg(conn):
    outboundMsg = raw_input("server> ")
    conn.sendall(outboundMsg)
    return outboundMsg

#this receives a message from the client
def recvMsg(conn):
    inboundMsg = conn.recv(buffer)
    if ("\quit" in inboundMsg):
	    return 1
    else:		
        print "{}{}".format(userName, inboundMsg)
        return 0
 
#this loop exists so that when a connection is closed iwth a client, it begins to listen for a new connection 
while(1):
    conn, addr = s.accept()
    userName = exchangeNames(conn)
    chat(conn)
    conn.close()



