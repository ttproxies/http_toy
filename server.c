#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>

#define BACKLOG 5

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "usage: ./server <port>\n");
        return 1;
    }

    const char *PORT = argv[1];
    const char *msg = "haloo !!!";

    // Initialize values for getaddrinfo()
    int gai_status;
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    // Query own address info
    if ((gai_status = getaddrinfo(NULL, PORT, &hints, &res)))
    {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(gai_status));
        return 1;
    }

    // Open socket
    int sockfd;
    if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1)
    {
        fprintf(stderr, "socket error: %s\n", strerror(errno)); // socket() sets errno
        return 1;
    }

    // Bind socket to port
    int b_status = bind(sockfd, res->ai_addr, res->ai_addrlen);
    if (b_status == -1)
    {
        fprintf(stderr, "bind error: %s\n", strerror(errno));
        return 1;
    }

    // Why would you connect here. Just gosh omg ur such a dumy.
    // Listen on port
    int l_status = listen(sockfd, BACKLOG);
    if (l_status == -1)
    {
        fprintf(stderr, "listen error: %s\n", strerror(errno));
        return 1;
    }

    printf("server is listening on port %s\n", PORT);

    // Accept pending connection from queue
    struct sockaddr_storage remote_addr;
    int new_fd = accept(sockfd, (struct sockaddr *)&remote_addr, (socklen_t *)sizeof(remote_addr)); // ugly ew
    if (new_fd == -1)
    {
        fprintf(stderr, "accept error: %s\n", strerror(errno));
        return 1;
    }

    // Communicate
    int len = sizeof(*msg), bytes_sent;
    do {
        bytes_sent = send(sockfd, msg, len, 0);
    } while (bytes_sent != len);


    return 0;
}