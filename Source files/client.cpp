#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

void client() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[1024];

    // Create client socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        cerr << "Socket creation failed." << endl;
        return;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080); // Convert port number to network byte order
    serverAddr.sin_addr.s_addr = inet_addr(" 192.168.0.103"); // Use localhost (127.0.0.1)

    // Connect to server
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        cerr << "Connection to server failed." << endl;
        return;
    }

    // Receive and display the messages from the server
    int n;
    while ((n = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
        buffer[n] = '\0'; // Null terminate the string
        cout << buffer;

        if (strstr(buffer, "final score")) break;  // End of game

        // Send guess to the server
        string guess;
        cout << "Enter your guess: ";
        cin >> guess;
        send(clientSocket, guess.c_str(), guess.size(), 0);
    }

    close(clientSocket);
}

int main() {
    client();
    return 0;
}
