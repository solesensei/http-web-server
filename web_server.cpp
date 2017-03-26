/* it's tcp/ip code carcass wrote on C */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  
#include <errno.h>
#include <string.h>   
#include <arpa/inet.h>    
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>

using namespace std;

#define TRUE   1
#define FALSE  0
#define PORT 5555 
#define BACK_LOG 3
#define MAXMSG 1024 //size of buffer 

int main()
{
    int opt = TRUE;
    int ms , addrlen , new_socket , client_socket[30] , max_clients = 30 , activity, i , valread , sd;
    int max_sd;
      
    char buffer[MAXMSG];

    fd_set readfds;
    
    //set array of sockets with 0
    for (i = 0; i < max_clients; i++) 
    {
        client_socket[i] = 0;
    }

    // master socket 
    if( (ms = socket(AF_INET , SOCK_STREAM , 0)) == 0) 
    {
        perror("socket creating failed");
        exit(EXIT_FAILURE);
    }

    //set master socket to allow multiple connections 
    if( setsockopt(ms, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
   
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    addrlen = sizeof(address);
      
    if ( bind(ms, (struct sockaddr *)&address, addrlen) < 0 ) 
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Listener on port %d \n", PORT);
     
    if (listen(ms, BACK_LOG) < 0)
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
      
    puts("Waiting for connections ...");
    
    char *message = "Session started!";
    for(;;)
    {
        FD_ZERO(&readfds); //clear set
        FD_SET(ms, &readfds); //add master socket to set
        max_sd = ms;
          
        //add child sockets to set
        for ( i = 0 ; i < max_clients ; i++) 
        {
            sd = client_socket[i];
             
            //if valid socket descriptor then add to read list
            if( sd > 0 )
                FD_SET( sd , &readfds );
             
            //compare socket descriptor on max value
            if(sd > max_sd)
                max_sd = sd;
        }
  
        //waiting for change on one of the sockets
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
    
        if ((activity < 0) && (errno!=EINTR)) //EINTR - call was interrupted
        {
            perror("select error");
        }
          
        //if master socket changed - incoming connection - accept it
        if (FD_ISSET(ms, &readfds)) 
        {
            if ((new_socket = accept(ms, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
            {
                perror("accept failed");
                exit(EXIT_FAILURE);
            }
          
            printf("New connection, socket fd : %d , ip : %s , port : %d \n" , \
                    new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
        
            //echo message to connected client

            if( send(new_socket, message, strlen(message), 0) != (int) strlen(message) ) 
            {
                perror("send failed");
            }
              
            printf("...welcome message sent\n");

            //add new socket
            for (i = 0; i < max_clients; i++) 
            {
                // searching on empty position
                if( client_socket[i] == 0 )
                {
                    client_socket[i] = new_socket;
                    printf("Adding to list of sockets as %d\n" , i);
                    /* some processing */
                    break;    
                }
            }
        } 
        //else - existing socket
        for (i = 0; i < max_clients; i++) 
        {
            sd = client_socket[i];

            if (FD_ISSET( sd , &readfds)) 
            {
                if ((valread = read( sd , buffer, MAXMSG)) == 0)
                {
                    //end of file - somebody disconnected

                    getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen); //address of peer connected to sd
                    printf("Host disconnected , ip : %s , port : %d" , 
                            inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
                      
                    shutdown(sd,2); // close connection
                    close( sd );
                    client_socket[i] = 0;
                }
                else //message forwarding - redirect
                {

                }
            }
        }
    }
    return 0;
}   
