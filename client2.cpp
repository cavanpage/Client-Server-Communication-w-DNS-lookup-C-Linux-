Client #2
#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write

struct datamembers{
    int a;
    char b;
    float c;
};

int main()
{
    int socket_desc, server_sock, sizeofsocketaddr , read_size;
    struct sockaddr_in server;
    struct datamembers mydata;
    char server_message[1000];  
    char *a, *b, *c;
    const char delim[2] = ",";
    /****************************************************************
                    Set up socket to listen to new
                    data from server at port 5060
    ***************************************************************/
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons((uint16_t)5060);
    //Create socket
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    //bind socket
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("bind failed. Error");
        return 1;
    } 
    //Listen
    listen(socket_desc , 3);
    //Accept and incoming connection
    puts("Waiting for server to send data...");
    sizeofsocketaddr = sizeof(struct sockaddr_in);  
    //accept connection from an incoming client
    server_sock = accept(socket_desc, (struct sockaddr *)&server, (socklen_t*)&sizeofsocketaddr);
    if (server_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
    puts("Connection accepted");
    memset( server_message, 0 , sizeof server_message);
    //Receive a message from client
    while( (read_size = recv(server_sock , server_message , sizeof(server_message), 0)) > 0 )
    {
       puts("Data from server");
       puts(server_message);
       //parse message from server
       a = strtok(server_message, delim);
       b = strtok(NULL, delim);
       c = strtok(NULL, delim);
       /*convert data to proper formats in
       the datamembers struct */
       sscanf(a, "%d", &mydata.a);
       sscanf(b, "%c", &mydata.b);
       sscanf(c ,"%f", &mydata.c);
       //output data from server that has been converted back to the proper struct
       printf("int: %d\tchar: %c\tfloat: %f\t\n", mydata.a, mydata.b, mydata.c);
       //reset message buffer from server
       memset( server_message, 0 , sizeof server_message);
    } 
    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    } 
    return 0;
}
