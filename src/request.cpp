#include "../include/request.h"
#include "../include/response.h"
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
using namespace std;

void handleClient(int clientFd) {
    char buffer[1024] = {0};
    read(clientFd, buffer, sizeof(buffer));

    string request(buffer);
    string firstline = request.substr(0, request.find("\r\n"));
    string req_method = firstline.substr(0, firstline.find(" "));
    string path = firstline.substr(firstline.find(" ") + 1);
    path = path.substr(0, path.find(" "));

    cout << "[INFO] " << req_method << " " << path << endl;

    string response = req_to_response(path, req_method);
    send(clientFd, response.c_str(), response.size(), 0);
    close(clientFd);
}
