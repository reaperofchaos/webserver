#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <map>
#include <vector>
#include <filesystem>  
#include <fstream>
#include "Directory.h"
#include "Request.h"

using namespace HttpRequest; 
using namespace std::filesystem;

sockaddr_in buildSocketAddress(int portNo);
Request createRequestFromClient(int connection);
std::string createResponseFromRequest(Request request, path sourceFolder);
void handleServiceError(int statusCode, int portNo);
void handleBindError(int webServerSocket, sockaddr_in socketAddress, int portNo);
void handleListenError(int webServerSocket, int maxPendingConnections);
void handleConnectionError(int connection);
std::string createMimeType(std::string extension);

int main(int argv, char** args){
    int portNo = 9999;
    if(argv > 1)
    {
        portNo = std::stoi(args[1]);
    }

    path workingDirectory = current_path();
	const std::string folderToSearch = "web"; 
    std::cout << "Starting Web Server.\n\nChecking for Web Folder\n";
	path sourceFolder = Directory::findFolderResults(folderToSearch, workingDirectory);
	path emptyPath; 
	if(sourceFolder == emptyPath){
        std::cout << "Web folder was not found in " << canonical(workingDirectory) << "\n"
                    << "Navigate to the project folder with the Web folder or create one and try again" << "\n"
                    << "Exiting....";
        return 1;
	}
	std::vector<fs::path> filesInDir = Directory::findFilesInDir(sourceFolder, false);

    int maxPendingConnections = 10; 
    int webServerSocket = socket(AF_INET, SOCK_STREAM, 0);
    handleServiceError(webServerSocket, portNo);

    sockaddr_in socketAddress = buildSocketAddress(portNo);
    int addressLength = sizeof(socketAddress);

    handleBindError(webServerSocket, socketAddress, portNo);
    handleListenError(webServerSocket, maxPendingConnections);

    std::cout << "\nServer is now accepting connections\n";
    //listen forever
    while(true){
        std::cout <<"\n+++++++ Waiting for new connection on Port "<< portNo << " ++++++++\n\n";
        int connection = accept(webServerSocket, (struct sockaddr*) &socketAddress, (socklen_t*)&addressLength);
        handleConnectionError(connection);
        Request request = createRequestFromClient(connection);
        

        std::cout << "------------------Response sent-------------------" << std::endl;
        std::string response = createResponseFromRequest(request, sourceFolder);

        write(connection, response.c_str(), strlen(response.c_str()));
        close(connection);
    }

    close(webServerSocket);
}   

sockaddr_in buildSocketAddress(int portNo)
{
    sockaddr_in socketAddress;
    socketAddress.sin_family = AF_INET;
    socketAddress.sin_addr.s_addr = INADDR_ANY;
    socketAddress.sin_port = htons(portNo); 
    memset(socketAddress.sin_zero, '\0', sizeof socketAddress.sin_zero);
    return socketAddress;
}

Request createRequestFromClient(int connection)
{
    char buffer[30000] = {0};
    read(connection, buffer, 30000);
    std::cout << "Message received: \n" << std::string(buffer, (sizeof(buffer))) << std::endl;
    int index = 0;
    std::string requestString; 
    std::string routeString; 

    //find request type
    while(buffer[index] != ' ' && index < (int)sizeof(buffer))
    {
        requestString.push_back(buffer[index]);
        index++; 
    }
    index++;

    //find route
    while(buffer[index] != ' ' && index < (int)sizeof(buffer))
    {
        routeString.push_back(buffer[index]);
        index++; 
    }
    Request newRequest = Request(Request::convertToRequest(requestString), routeString);
    return newRequest;
}


std::string createResponseFromRequest(Request request, path sourceFolder)
{
    std::string response = "HTTP/1.1";
    //std::cout << "Request type " << HttpRequest::Request::requestToString(request.getRequest()) << "\n"; 
    std::string route = request.getRoute();
    route.erase(0,1);
    switch(request.getRequest())
    {
        case RequestType::GET:
        {
            path found = Directory::findFilePath(route, sourceFolder);
            
            if(!found.empty()){
                //open the file
                std::ifstream file;
                file.open(__path_string(found), std::ios::in);
                std::string contents((std::istreambuf_iterator<char>(file)), 
                std::istreambuf_iterator<char>());

                response +=  " 200 OK\n";
                response += "Content-Type: " + createMimeType(found.extension()) + "\n";
                response += "Content-Length: " + std::to_string(contents.length()) + "\n\n";
                response += contents + "\n";
                std::cout << "Response: \n" << response << "\n"; 
            }else{
                response +=  " 404 OK\nContent-Type: text/html\nContent-Length: 100\n\n <html><div style=\"display:flex;flex-direction:row;justify-content:center;\"><h1>Page not found</h1></html>";
            }

        break;
        }
        default:
        {
        // default case

        }
    };
    return response; 
}

    void handleServiceError(int statusCode, int portNo)
    {
        if(statusCode == -1)
        {
            std::cout << "Failed to bind to port " << portNo <<". errno: "<< errno << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    void handleBindError(int webServerSocket, sockaddr_in socketAddress, int portNo)
    {
        if(bind(webServerSocket, (struct sockaddr*)&socketAddress, sizeof(socketAddress)) < 0){
            std::cout << "Failed to bind to port " << portNo <<". errno: "<< errno << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    void handleListenError(int webServerSocket, int maxPendingConnections)
    {
        if(listen(webServerSocket, maxPendingConnections) < 0){
            std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    void handleConnectionError(int connection)
    {
        if(connection < 0){
            std::cout << "Failed to grab connection. errno: " << errno << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    std::string createMimeType(std::string extension)
    {
        std::string mimeType = "text/plain";
        if(extension == ".html")
        {
            mimeType = "text/html";
        }
        return mimeType;
    }