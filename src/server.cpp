#include "../include/server.h"
#include "../include/request.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <atomic>
#include <csignal>
using namespace std;

extern atomic<bool> running;
extern int serverFd;

void startServer(int port) {
    serverFd = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    bind(serverFd, (struct sockaddr*)&address, sizeof(address));
    listen(serverFd, 10);

    cout << "Server listening! http://localhost:" << port << endl;

    while (running) {
        int clientFd = accept(serverFd, nullptr, nullptr);
        if (clientFd < 0) break;
        thread t(handleClient, clientFd);
        t.detach();
    }
}