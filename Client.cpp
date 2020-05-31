
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>  
#include <bits/stdc++.h>

using namespace std;

int main(){

    //socket()  and read () returns int value > 0?
    int socketDesriptor, portNumber, sockInt;
    struct sockaddr_in serverAddress;
    struct hostent *server;
    char buffer[255] = {0};  

    string hostName;
    cout << " Enter host name:  ";
    cin >> hostName;
    
    cout <<endl<< " Enter port number: ";
    std::cin >> portNumber;
  
   // creates the socket, 
    socketDesriptor = socket(AF_INET, SOCK_STREAM, 0);
    if(socketDesriptor < 0){
        perror("Failed to open socket");
        exit(1);
    }

    server = gethostbyname(hostName.c_str());

    if(server == NULL){
        fprintf(stderr, "Error , no such HOst");
    } 
    
    bzero ( (char *) &serverAddress, sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;

    bcopy((char *) server->h_addr, (char *) &serverAddress.sin_addr.s_addr, server->h_length);

    serverAddress.sin_port = htons(portNumber);

    if(connect(socketDesriptor, (struct sockaddr *) &serverAddress, sizeof(serverAddress))< 0){

        perror("conncection failed");
        exit(1);
    }

std::string cityName;
//input for the city Name
std::string input="";

bzero(buffer, 255);

sockInt = read(socketDesriptor, buffer, 255);

printf(" %s\n", buffer);
bzero(buffer, 255);
//cin.ignore only reads the first line with cin >>
cin.ignore(1);
std::getline(std::cin, input);


sockInt = write(socketDesriptor, input.c_str(), 255);
bzero(buffer, 255);

//read the result in buffer
sockInt = read(socketDesriptor, buffer, 255);
std::cout << std::endl << " --Weather report for " << input.c_str() << std::endl;

std::string name = "";
name += buffer;
std::cout << buffer <<std::endl;

close(socketDesriptor);
return 0;
}