#!/usr/bin/env python

from socket import *
import sys

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print 'Please enter a port number to listen on'
        exit(1)
        
port = sys.argv[1]
serverName = "server"


buffer = 1024

s = socket(AF_INET, SOCK_STREAM)
s.bind(('', int(port)))
s.listen(1)

def exchangeNames(conn):
    userName = conn.recv(buffer)
    userName = userName + "> "
    conn.send(serverName)
    return userName
    
def chat(conn):
    while(1):
        recvMsg(conn)
        sendMsg(conn)
    
def sendMsg(conn):
    outbound = raw_input("{}> ".format(serverName))
    print outbound

def recvMsg(conn):
    inbound = conn.recv(buffer)
    print inbound
    
while(1):
    conn, addr = s.accept()
    userName = exchangeNames(conn)
    chat(conn)
    conn.close()



