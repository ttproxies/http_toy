#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>

#define PORT "3490"

int main(int argc, char *argv[])
{
    // Initialize values for getaddrinfo()
    int gai_status;
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    // Query address info
    if ((gai_status = getaddrinfo(NULL, PORT, &hints, &res)))
    {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(gai_status));
        return 1;

    }

    // Open socket
    int s;
    if ((s = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1)
    {
        fprintf(stderr, "socket error: %s\n", strerror(errno)); // socket() sets errno
        return 2;
    }



    return 0;
}