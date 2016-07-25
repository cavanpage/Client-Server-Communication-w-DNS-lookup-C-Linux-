#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<string.h>
#include<netdb.h>
/*structure that contains data
to send to server */
struct datamembers{
    int a;
    char b;
    float c;
};


int hostname_to_ip(char *, char *);


int main(int argc, char *argv[])
{
    struct datamembers mydata;
    struct sockaddr_in server;
    int sock;
    char message[1000];
    char ip[100];

    /*****************************************
        set data variables in our custom structure
    *****************************************/
    mydata.a = 2;
    mydata.b = 'A';
    mydata.c = 4.5;

    /*check to see if server name is included 
    as a command line argument */

    if( argc < 2){
        puts("Please server name from one of the following");
        puts("volta.sdsu.edu");
        puts("rohan.sdsu.edu");
        puts("jason.sdsu.edu");
        return 1;
    }
    char *hostname = argv[1];
    hostname_to_ip(hostname, ip);
    /*****************************************
        write struct variables to a single string
        for easy data transmission across socket
         -- separate using commas 
         -- this will be parsed by the server
    *****************************************/
    sprintf(message,"%d,%c,%f", mydata.a, mydata.b, mydata.c);
    /*****************************************
            Create the socket
    ****************************************/
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)printf("Could not create socket");
    /*declare socket properties using converted 
    ip (from hostname) and open socket (5050) */
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_family = AF_INET;
    server.sin_port = htons((uint16_t)5050);
    //print struct variables to ensure data is written correctly
    printf("message (original): int: %d, char: %c, float: %f\n", mydata.a, mydata.b, mydata.c);
    //connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }  
    printf("Connected to: %s (%s)\n", hostname, ip);
    //send the same data every 4 seconds
    while(1)
    {
        sleep(4);   
        printf("Sending....\n") ; 
        //send formatted data buffer 
        if( send(sock , message , strlen(message) , 0) < 0)
        {
            puts("Send failed");
            return 1;
        }      
    }   
    close(sock);
    return 0;
}


/*This function converts the hostname 
to the proper ip address using a 
dns query  http://www.binarytides.com/hostname-to-ip-address-c-sockets-linux/ */
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
 
