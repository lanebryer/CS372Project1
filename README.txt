To execute the program:

1) Compile the C client with: gcc -o tcpclient tcpclient.c

2) Run the tcp server with the command: ./tcpserver.py <portnum>, where <portnum> is a valid port number

3) If the server does not run, execute the command: chmod +x tcpserver.py and then try step 2 again

4) Run the tcp client with the command: ./tcpclient <hostname> <portnum>, where <hostname> is the url or IP address of the server and <portnum> is the port
the server is running on.

5) Enter a user name in the client process and then enter a message in the client process to send to the server

6) Enter a message in the server process to send back to the client

7)Type \quit in either the server or client to close the client and return the server to listening for a new connection