#include <string>
#include <iostream>
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <pthread.h>
#include <cstdlib>  

#define PORT 8080 
#define MESSAGE_SIZE 1024

using namespace std;
   
void *read_socket(void *client_sock);

int main(int argc, char const *argv[]) 
{ 
    int client_socket = 0, valread; 
    struct sockaddr_in serv_addr; 

    char buffer[1024] = {0}; 
    
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(client_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 
    
    pthread_t tid;
    int rc = pthread_create(&tid, NULL, read_socket, (void *)client_socket); 
    if (rc) {
         printf("Error:unable to create thread, %d\n", rc);
         exit(-1);
    }

    string join_string = "/name mamad";
    send(client_socket , join_string.c_str() , strlen(join_string.c_str()) , 0 ); 

    string user_command = "";

    do{
        cin >> user_command;
        send(client_socket, user_command.c_str(), strlen(user_command.c_str()), 0);
    }while(user_command.find("/quit") == string::npos);
    
    pthread_exit(NULL); 
    printf("[CLIENT] Quit called\n"); 
    // valread = read( client_socket , buffer, 1024); 
    // printf("%s\n",buffer ); 

    return 0; 
} 
void *read_socket(void *client_sock){
    int client_socket = (int)(long)client_sock;
    char buffer[MESSAGE_SIZE];
    while(1){
        int byte_count = read(client_socket, buffer, MESSAGE_SIZE);
        
        if(byte_count > 0){
            cout << "read:" << byte_count <<endl;
            cout << buffer << endl;
 
        }
       
    }

    

}