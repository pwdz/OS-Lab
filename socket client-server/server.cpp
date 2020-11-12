#include <string>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <list> 
#include <thread>
#include <iostream>

using namespace std;

// #define PORT 8080
#define MESSAGE_SIZE 1024

void checkkErr(int statusCode, string msg);

void *handle_client(void *client_sock);
   
int main(int argc, char *argv[]) {
    cout << "asd";
    if(argc < 2){
        puts("[ERROR]Not enough args");
        exit(EXIT_FAILURE);
    }
    int port = atoi(argv[1]);
    int server_fd;

    struct sockaddr_in address;
 
    server_fd = socket(AF_INET, SOCK_STREAM, 0);//0 -> choose protocol automatically

    address.sin_family = AF_INET;//IP_V4
    address.sin_addr.s_addr = INADDR_ANY;//addresses to accept any incoming messages
    address.sin_port = htons(port);

    const int addrlen = sizeof(address);
    
    checkkErr(bind(server_fd, (struct sockaddr *)&address, sizeof(address)), "Binding failed");

    checkkErr(listen(server_fd, 3), "Listen error");
    printf("Listen on %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

    cout << ":|";
    // while (true){

        cout << "asdadadadadadadadadadad11111111";
        int new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        checkkErr(new_socket, "accept"); 

        cout << "asdadadadadadadadadadad";
        pthread_t tid;
        int rc = pthread_create(&tid, NULL, handle_client, (void *)new_socket); 
        if (rc) {
            printf("Error:unable to create thread, %d\n", rc);
            exit(-1);
        }
    // }
    
  
   
    // printf("%s\n",buffer ); 
    // send(new_socket , "salammmmmm" , strlen("salammmmmm") , 0 ); 

    // writes to client socket
    return 0;
}    
void checkkErr(int statusCode, string msg){
    if(statusCode < 0){
        perror(msg.c_str());
        exit(EXIT_FAILURE);
    }
}
void *handle_client(void *client_sock){

    cout << "s:||||||";
    int client_socket = (int)(long)client_sock;

    cout << "s:||||||";
    char buffer[MESSAGE_SIZE];
    int byte_count;
        string user_command = "";

    do{
        byte_count = read( client_socket , buffer, MESSAGE_SIZE); 
        if(byte_count > 0){
            string input(buffer);
            cout << input;

            send(client_socket, input.c_str(), strlen(input.c_str()), 0);
        }

        // send(client_socket, user_command.c_str(), strlen(user_command.c_str()), 0);
    }while(user_command.find("/quit") == string::npos);
 
  

}