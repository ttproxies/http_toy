#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char *argv[]) {
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./server.c <host>\n");
        return 1;
    }
    
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
        return 1;
    }

    char addr_buf[100];

    if (servinfo->ai_family == AF_INET)
    {
        struct sockaddr_in *sa = (struct sockaddr_in *)servinfo->ai_addr;
        inet_ntop(AF_INET, &(sa->sin_addr), addr_buf, INET_ADDRSTRLEN);
    }
    else
    {
        struct sockaddr_in6 *sa6 = (struct sockaddr_in6 *)servinfo->ai_addr;
        inet_ntop(AF_INET6, &(sa6->sin6_addr), addr_buf, INET6_ADDRSTRLEN);
    }

    printf("IP for %s:\n", host);
    printf("%s\n", addr_buf);

    freeaddrinfo(servinfo);
}