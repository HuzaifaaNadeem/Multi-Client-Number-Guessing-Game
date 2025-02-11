#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

void handleClient(int clientSocket) {
    char buffer[1024];
    int score = 0;

    // Send welcome message
    send(clientSocket, "Welcome to the Number Guessing Game! Guess a number between 1 and 100.\n", 69, 0);
    send(clientSocket, "You have 5 rounds to play. Let's start!\n", 39, 0);

    srand(time(0));

    for (int round = 1; round <= 5; round++) {
        int randomNumber = rand() % 100 + 1; // Random number between 1 and 100
        send(clientSocket, "Guess the number: ", 19, 0);

        int guess;
        int n = recv(clientSocket, buffer, sizeof(buffer), 0);
        buffer[n] = '\0'; // Null terminate the received string
        guess = atoi(buffer);

        if (guess == randomNumber) {
            score++;
            send(clientSocket, "Correct! You gained a point.\n", 28, 0);
        } else {
            send(clientSocket, "Wrong! Better luck next time.\n", 30, 0);
        }
    }

    // Send final score to client
    string finalScore = "Your final score is: " + to_string(score) + "\n";
    send(clientSocket, finalScore.c_str(), finalScore.size(), 0);

    // Close the connection
    close(clientSocket);
}

void server() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    // Create server socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        cerr << "Socket creation failed." << endl;
        return;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;  // Bind to all available network interfaces
    serverAddr.sin_port = htons(8080);  // Convert port number to network byte order

    // Bind the socket
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        cerr << "Binding failed." << endl;
        return;
    }

    // Listen for incoming connections
    if (listen(serverSocket, 5) == -1) {
        cerr << "Listening failed." << endl;
        return;
    }

    cout << "Server is waiting for clients..." << endl;

    // Accept a client connection
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
    if (clientSocket == -1) {
        cerr << "Client connection failed." << endl;
        return;
    }

    handleClient(clientSocket);  // Handle the client

    // Close the server socket
    close(serverSocket);
}

int main() {
    server();
    return 0;
}
