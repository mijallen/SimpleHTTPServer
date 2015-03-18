#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"

// check if system is Windows //

#ifdef _WIN32
#define _WINDOWS_DETECTED_
#endif

// conditional fixes for unistd on Windows //

#ifdef _WINDOWS_DETECTED_
  #define off_t _off_t
  #define off64_t _off64_t
#endif

#include <unistd.h>
#include <dirent.h>

// conditional includes and definitions for sockets //

#ifdef _WINDOWS_DETECTED_
  #include <winsock.h>
  #define CLOSESOCKET closesocket
  WSADATA wsaData;
#else
  #include <arpa/inet.h>
  #define CLOSESOCKET close
#endif

// server options //

#define BUFFER_SIZE 4096
#define BACKLOG_SIZE 5

char *defaultHeader = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 11\r\nConnection: Close\r\n\r\n";
char *defaultBody = "Hello World";

char *errorHeader = "HTTP/1.1 404 NOT FOUND\r\nContent-Type: text/html\r\nContent-Length: 5\r\nConnection: Close\r\n\r\n";
char *errorBody = "ERROR";

// error-handling code //

enum ErrorCode {
    EC_NO_ERROR,
    EC_WSASTARTUP_FAILURE,
    EC_SOCKET_FAILURE,
    EC_BIND_FAILURE,
    EC_LISTEN_FAILURE,
    // error codes here //
    EC_ERROR_CODE_COUNT
};

void printWarning(char *message) {
    fprintf(stderr, "Warning: %s\n", message);
}

void exitWithError(char *message, enum ErrorCode status) {
    fprintf(stderr, "ERROR: %s\n", message);
    exit(status);
}

// utility functions //

void printWithEscapeCharacters(FILE *out, char *message) {
    unsigned int index = 0;
    char currentCharacter = message[index];

    while (currentCharacter != '\0') {
        switch (currentCharacter) {
        case '\r':
            fputc('\\', out);
            fputc('r', out);
            break;
        case '\n':
            fputc('\\', out);
            fputc('n', out);
            break;
        case '\t':
            fputc('\\', out);
            fputc('t', out);
            break;
        default :
            fputc(currentCharacter, out);
            break;
        }

    index += 1;
    currentCharacter = message[index];
    }
}

// server configuration functions //

void checkArguments(int argc, char **argv, ConfigurationRef config) {
    argc = argc;
    argv = argv;

    if (argc > 2) {
        configLoadFile(config, argv[1]);
    }
}

int setupServer(ConfigurationRef config) {
    int socketNumber = 0;
    int status = 0;
    struct sockaddr_in configuration = (const struct sockaddr_in){0};

    socketNumber = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socketNumber < 0) {
        exitWithError("setupServer: unable to obtain socket for server",
            EC_SOCKET_FAILURE);
    }

    memset(&configuration, 0, sizeof(configuration));
    configuration.sin_family = AF_INET;
    configuration.sin_addr.s_addr = htonl(INADDR_ANY);
    configuration.sin_port = htons(configPort(config));

    status = bind(socketNumber, (struct sockaddr*)&configuration, sizeof(configuration));
    if (status < 0) {
        exitWithError("setupServer: socket could not be bound with configuration",
            EC_BIND_FAILURE);
    }

    status = listen(socketNumber, BACKLOG_SIZE);
    if (status < 0) {
        exitWithError("setupServer: unable to configure server listening",
            EC_LISTEN_FAILURE);
    }

    return socketNumber;
}

void handleClientRequest(int clientSocket, char* request) {
    printf("received message:\n");

    printWithEscapeCharacters(stdout, request);
    printf("\n");

    printf("\n");

    printf("sent message:\n");

    send(clientSocket, defaultHeader, strlen(defaultHeader), 0);
    printWithEscapeCharacters(stdout, defaultHeader);
    printf("\n");

    send(clientSocket, defaultBody, strlen(defaultBody), 0);
    printWithEscapeCharacters(stdout, defaultBody);
    printf("\n");

    printf("\n");

    CLOSESOCKET(clientSocket);
}

int main(int argc, char **argv) {
    int serverSocket = 0;
    int clientSocket = 0;
    int receivedCharacters = 0;
    char messageBuffer[BUFFER_SIZE];
    ConfigurationRef config = configCreate();

    checkArguments(argc, argv, config);

    // conditional calls to winsock methods //
    #ifdef _WINDOWS_DETECTED_
      int status = WSAStartup(MAKEWORD(2, 0), &wsaData);
      if (status != 0) {
          exitWithError("main: could not perform startup on Windows",
              EC_WSASTARTUP_FAILURE);
      }
    #endif

    serverSocket = setupServer(config);

    printf("awaiting client connections...\n\n");

    while (1) {
        clientSocket = accept(serverSocket, NULL, NULL);
        printf("client connected!\n\n");

        receivedCharacters = recv(clientSocket, messageBuffer, BUFFER_SIZE, 0);
        if (receivedCharacters < 1) {
            printf("---ERROR RETRIEVING CLIENT REQUEST---\n");
            if (receivedCharacters == 0) printf(" -client already shutdown\n");
            if (receivedCharacters < 0) printf(" -socket error, check errno\n");
            //CLOSESOCKET(clientSocket);
            continue;
        }

        handleClientRequest(clientSocket, messageBuffer);
    }

    CLOSESOCKET(serverSocket);

    // conditional calls to winsock methods //
    #ifdef _WINDOWS_DETECTED_
      WSACleanup();
    #endif

    return 0;
}
