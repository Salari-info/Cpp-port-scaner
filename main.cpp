#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

bool isPortOpen(const string& ip, int port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        cerr << "Unable to create socket" << endl;
        return false;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &server.sin_addr);

    // Set a timeout for the connection attempt
    struct timeval timeout;
    timeout.tv_sec = 1;  // 1 second
    timeout.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));

    // Attempt to connect
    int result = connect(sock, (struct sockaddr*)&server, sizeof(server));
    close(sock);

    return result == 0;  // If connect returns 0, the port is open
}


int main(int argc, char* argv[]) {

	const string RESET = "\033[0m";  
    const string RED = "\033[31m";   
    const string GREEN = "\033[32m"; 
    const string WHITE = "\033[37m"; 


    string ip;
    int startPort;
    int endPort;


    if (argc > 1 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)) {
        cerr << WHITE << "Usage: " << argv[0] << " <IP address> <start port> <end port>" << endl;
        return 1;
    }


    if (argc != 4) {
        cout <<"Enter IP address: ";
        cin >> ip;
        cout << "Enter start port: ";
        cin >> startPort;
        cout << "Enter end port: ";
        cin >> endPort;
    } else {
        ip = argv[1];
        startPort = stoi(argv[2]);
        endPort = stoi(argv[3]);
    }

    if (startPort < 0 || endPort < 0 || startPort > endPort) {
        cerr << "Invalid port range." << endl;
        return 1;
    }
    for (int port = startPort; port <= endPort; ++port) {
        if (isPortOpen(ip, port)) {
            cout << GREEN << "Port " << port << " is open." << endl;
        } else {
            cout << RED << "Port " << port << " is closed." << endl;
        }
    }

    return 0;

}
