// Simple demo file for basic socket stuff

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./ip.c <host>\n");
        return 1;
    }

    const char *host = argv[1];

    int status;
    struct addrinfo hints;     // pre-written struct for getaddrinfo to use
    struct addrinfo *servinfo; // pointer to linked list of results

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; // IP version agnostic
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(host, "3490", &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        return 1;
    }

    printf("all associated IP addresses for %s:\n", host);

    for (struct addrinfo *p = servinfo; p != NULL; p = p->ai_next)
    {
        char *ipver;
        char ipstr[INET6_ADDRSTRLEN];

        if (p->ai_family == AF_INET)
        {
            ipver = "IPv4";
            struct sockaddr_in *sa = (struct sockaddr_in *)p->ai_addr;
            inet_ntop(AF_INET, &(sa->sin_addr), ipstr, INET_ADDRSTRLEN);
        }
        else
        {
            ipver = "IPv6";
            struct sockaddr_in6 *sa6 = (struct sockaddr_in6 *)p->ai_addr;
            inet_ntop(AF_INET6, &(sa6->sin6_addr), ipstr, INET6_ADDRSTRLEN);
        }

        printf("%s for %s: %s\n", ipver, host, ipstr);
    }

    freeaddrinfo(servinfo);

    return 0;
}