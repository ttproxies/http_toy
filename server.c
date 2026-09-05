#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char *argv[]) {
    const char *host = argv[1];
    
    int status;
    struct addrinfo hints; // pre-written struct for getaddrinfo to use
    struct addrinfo *servinfo; // pointer to linked list of results

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; // IP version agnostic
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(host, "3490", &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    }

    const char *output = servinfo->ai_addr->sa_data;

    printf("IP for %s:\n", host);
    printf("%s\n", output);
}