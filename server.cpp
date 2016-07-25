#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<netdb.h>

int hostname_to_ip(char *, char *);

int main(int argc, char *argv[])
{
    int socket_desc , client_sock ,client_sock_2, sizeofsocketaddr , read_size;
    struct sockaddr_in server , client, clientfinal;
    char client_message[1000], message_altered[1000];
    //variables used to parse message and alter data
    char *a, *b, *c;
    int aint;
    char bchar;
    float cfloat;
    const char delim[2] = ",";
    char ip[100];
    /*check to see if server name is included as 
    command line argument */
    if( argc < 2){
        puts("Please client name from one of the following");
        puts("volta.sdsu.edu");
        puts("rohan.sdsu.edu");
        puts("jason.sdsu.edu");
        return 1;
    }
    char *hostname = argv[1];
    hostname_to_ip(hostname, ip);
    /**************************************************************
                     set up socket to connect to client #2
                     using socket 5060
    ***************************************************************/
    client_sock_2 = socket(AF_INET, SOCK_STREAM, 0);
    if(client_sock_2 == -1) printf("could not create socket");
    clientfinal.sin_addr.s_addr = inet_addr(ip);
    clientfinal.sin_family = AF_INET;
    clientfinal.sin_port = htons((uint16_t)5060);
    /****************************************************************
                    Set up server socket to accept any 
                    incoming client on port 5050
    ***************************************************************/
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons((uint16_t)5050);
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
    listen(socket_desc , 3);
    puts("Waiting for client to send data...");
    sizeofsocketaddr = sizeof(struct sockaddr_in);
    //accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&sizeofsocketaddr);
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
    puts("Connection accepted from incoming client");
    memset( client_message, 0 , sizeof client_message);
    //try and connect to client #2
        if(connect(client_sock_2, (struct sockaddr*)&clientfinal, sizeof(clientfinal)) < 0){
            perror("cannot connect to client #2: Error");
        }
    printf("Connected to: %s (%s)\n", hostname, ip);
    //Receive a message from client
    while( (read_size = recv(client_sock , client_message , sizeof(client_message), 0)) > 0 )
    {
       //parse the message (by a comma) into proper data formats
       printf("Original Data: \t%s\n", client_message);
       a = strtok(client_message, delim);
       b = strtok(NULL, delim);
       c = strtok(NULL, delim);
       //convert strings to proper datatypes
       sscanf(a, "%d", &aint);
       sscanf(b, "%c", &bchar);
       sscanf(c, "%f", &cfloat);
       //alter the data as specified by the assignment
       aint = aint * 2; // multiply int by 2
       bchar = bchar + 1; // increase char value to next letter
       cfloat = cfloat + 1; // add 1 to float value
        //write altered data to new message buffer
        sprintf(message_altered,"%d,%c,%f", aint, bchar, cfloat);
        printf("Altered Data: \t%s\n", message_altered);
        //try to send/forward message to client #2
        if(send(client_sock_2, message_altered, strlen(message_altered), 0) < 0){
            puts("cannot send to client #2: Error");
        }
        puts("Received...Altered...Sent to client #2");
        //reset message buffers so we can recieve fresh data from clients
        memset( client_message, 0 , sizeof client_message);
        memset( message_altered, 0, sizeof message_altered);
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
/*This function converts the hostname 
to the proper ip address using a 
dns query */
int hostname_to_ip(char * hostname , char* ip)
{
    struct hostent *he;
    struct in_addr **addr_list;
    int i;
        
    if ( (he = gethostbyname( hostname ) ) == NULL) 
    {
        // get the host info
        puts("gethostbyname");
        return 1;
    }
    addr_list = (struct in_addr **) he->h_addr_list;
    
    for(i = 0; addr_list[i] != NULL; i++) 
    {
        //Return the first one;
        strcpy(ip , inet_ntoa(*addr_list[i]) );
        return 0;
    }  
    return 1;
}
