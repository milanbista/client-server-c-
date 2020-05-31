# client-server-c-
A program to demonstrate Client and Server connection and communication using c++ language
This program consist of three files Server.cpp, Client.cpp and inputfile(weather.txt)

Make sure you compile the code and run it in two different terminal tabs.

#weather.txt
weather.txt is a text file consisting of cityName, temperature and the sky condition separated by comms(,)
Boston, 45, Sunny
New York, 23, Clouds

#Server.cpp
compile Server.cpp (g++ Server.cpp) and run (./a.out). The program will read the input weather.txt and store it. 
The server will then be running with a port number assigned and start listening to incoming client request.

#Client.cpp
compile Client.cpp (g++ Client.cpp) and run (./a.out). The program will as for the host (localhost) and port number. If the port number 
mateches with the port number assigned to Server.cpp, the connection is made and Client can ask for the city information and Server will 
provide the information based on the client request.
