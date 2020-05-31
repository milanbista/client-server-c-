
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
//socket
#include <sys/socket.h>
// internet socket
#include <netinet/in.h>
#include <iostream>
#include <fstream>
//transform the string into lower or uppercase
#include <bits/stdc++.h>

// structure for city records contains city name, temperature and sky condition
struct cityRecord
{
    std::string cityName;
    std::string temperature;
    std::string skyCondition;
};

//read the input form weather.txt
// getline is looped through to store name, tem, and sky
// seperated by 'comma'
int readFile(cityRecord *city)
{
    int cityCount = 0; // number of city
    std::ifstream inputFile;
    inputFile.open("weather.txt");
    std::string inputLine;
    int counter = 0;

    while (!inputFile.eof())
    {
        std::getline(inputFile, inputLine);
        cityCount++;
        int index = 0;
        for (int i = 0; i < inputLine.size(); i++)
        {
            if (index == 0 && inputLine[i] != ',')
            {
                index = 0;
                city[counter].cityName += inputLine[i];
            }

            else if (index == 1 && inputLine[i] != ',')
            {
                index = 1;
                city[counter].temperature += inputLine[i];
            }

            else if (index == 2 && inputLine[i] != ',')
            {
                index = 2;
                city[counter].skyCondition += inputLine[i];
            }

            else
            {
                index++;
            }
        }
        counter++;
    }
    return cityCount;
}

// comapre the client input cityName with the city stored in Server
int compareCities(std::string inputName, cityRecord *city, int numberofCity)
{
    for (int i = 0; i < numberofCity; i++)
    {
        // compare the client input name with the cityName in server
        // strcmp funtion returns 0 if the two string are same
        // transform transforms the given string into upper or lower case
        std::transform(inputName.begin(), inputName.end(), inputName.begin(), ::tolower);
        std::string currentCityName = city[i].cityName;
        std::transform(currentCityName.begin(), currentCityName.end(), currentCityName.begin(), ::tolower);

        if (strcmp(inputName.c_str(), currentCityName.c_str()) == 0)
        {
            return i;
        }
    }
    //if the city doesnot match the record returns -1
    return -1;
}

//program begins here !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
int main()
{
    int numberOfCity = 0;
    cityRecord city[10];

    //call the read function to read the input text file
    numberOfCity = readFile(city);
    //buffer is passed through read write
    char buffer[255] = {0};
    int opt = 1;
    int socketDescriptor, newSocketDes;
    int portNumber, sockInt;

    //sockaddr_in gives the internet addresss
    struct sockaddr_in serverAddress, clientAddress;
    //gives the size of clientaddress
    socklen_t clientSize;

    std::cout << " Enter port NUmber: ";
    std::cin >> portNumber;

    //creating the Socket which return the int value
    socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socketDescriptor < 0)
    {
        //error("Error Opening Socket!!!");
        perror("Error occured while creating the socket");
        exit(1);
    }

    // reset the values
    bzero((char *)&serverAddress, sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(portNumber);

    if (setsockopt(socketDescriptor, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("Sorry Socket Failed!!");
        exit(1);
    }

    if (bind(socketDescriptor, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        perror("Error occured while Binding !!");
        exit(1);
    }

//listenAgain is the loop point which will be called again
// so the server keeps running for other clients
listenAgain:
    listen(socketDescriptor, 5);
    clientSize = sizeof(clientAddress);

    newSocketDes = accept(socketDescriptor, (struct sockaddr *)&clientAddress, &clientSize);

    if (newSocketDes < 0)
    {
        perror("Error while accepting the Socket request !!");
        exit(1);
    }

    std::string inputName;
    bzero(buffer, 255);

    // write sends the buffer to the client
    sockInt = write(newSocketDes, "Enter the city name: ", strlen("Enter the city name: "));

    bzero(buffer, 255);
    //reads the request from client
    read(newSocketDes, buffer, 255);

    std::string currentCity = "";
    currentCity += buffer;
    std::cout << std::endl
              << " --Weather report for " << currentCity << std::endl;
    int matchIndex = compareCities(currentCity, city, numberOfCity);

    if (matchIndex == -1)
    {
        std::cout << " No Data ! " << std::endl;
        write(newSocketDes, "-- No Data! --", 255);
    }
    else
    {
        std::string result = "--Tomorrows maximum temperature is " + city[matchIndex].temperature + " F \n " + "--Tomorrows sky condition is " + city[matchIndex].skyCondition;
        std::cout << result << std::endl;
        write(newSocketDes, result.c_str(), 255);
    }

    goto listenAgain;
    close(newSocketDes);
    close(socketDescriptor);
    return 0;
}