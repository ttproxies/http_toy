#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#define BACKLOG 5

void sigchld_handler(int s)
{
    (void)s;
    int tmp_errno = errno;
    while (waitpid(-1, NULL, WNOHANG) > 0); // Claim the souls of whatever child process has finisbed
    errno = tmp_errno;
}

struct sockaddr *get_in_addr(struct sockaddr_storage *saddr)
{
    if (saddr->ss_family == AF_INET)
        { return (struct sockaddr_in *)saddr; }
    if (saddr->ss_family == AF_INET6)
        { return (struct sockaddr_in6 *)saddr; }
}

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
    int gai_status, sockfd, newfd;
    int yes = 1;
    struct sigaction sa;
    struct addrinfo hints, *res, *cur;
    struct sockaddr_storage *their_addr;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    // Query own address info
    if ((gai_status = getaddrinfo(NULL, PORT, &hints, &res)))
    {
        perror("getaddressinfo");
        return 1;
    }

    for (cur = res; cur != NULL; cur = cur->ai_next)
    {
        if ((sockfd = socket(cur->ai_family, cur->ai_socktype, cur->ai_protocol)) == -1)
        {
            perror("socket");
            continue;
        }

        // do more research on this
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
        {
            perror("setsockopt");
            return 1;
        }

        if ((newfd = bind(sockfd, cur->ai_addr, cur->ai_addrlen)) == -1)
        {
            perror("bind");
            continue;
        }

        break;
    }

    // we don't need this anymore
    freeaddrinfo(res);

    if (cur == NULL)
    {
        perror("could not bind");
        return 1;
    }

    if (listen(sockfd, BACKLOG) == -1)
    {
        perror("listen");
        return 1;
    }

    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGCHLD, &sa, NULL) == -1) // mount new sigaction
    {
        perror("sigaction");
        return 1;
    }

    // accept loop
    while(1) {
        if ((newfd = accept(sockfd, get_in_addr(their_addr), sizeof their_addr)) == -1)
        {
            perror("accept");
            continue;
        }

        if (!fork()) { // child process
            close(sockfd);
            send(newfd, msg, strlen(msg), 0);
            close(newfd);
            exit(0);
        }

        close(newfd);
    }

    return 0;
}