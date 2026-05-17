#include <iostream>
#include <csignal>
#include <atomic>
#include <unistd.h>
#include "../include/server.h"
#include <include/server.h>
using namespace std;

atomic<bool> running(true);
int serverFd;

void signalHandler(int sig) {
    cout << "\n[INFO] Shutdown signal received. Stopping server..." << endl;
    running = false;
    close(serverFd);
    serverFd = -1;
}

int main() {
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    startServer(8080);
    return 0;
}