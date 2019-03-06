#include "reactor.h"
#include "tnet.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


Reactor *pReactor;

int generic_accept(int fd, struct sockaddr *sa, socklen_t *len)
{
    int ret;
    while (1)
    {
        ret = accept(fd, sa, len);
        if (ret == -1)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                continue;
            }
            else
            {
                printf("accept err!");
                break;
            }
        }
        break;
    }
    return ret;
}

int server_accept(int fd, char *ip, size_t ip_len, int *port)
{
    sockaddr_in cli_addr;
    socklen_t cliaddr_len = sizeof(cli_addr);
    int ret = generic_accept(fd, (struct sockaddr *)&cli_addr, &cliaddr_len);
    if (ret == -1)
    {
        return -1;
    }
    if (ip)
    {
        inet_ntop(AF_INET, (void *)&(cli_addr.sin_addr), ip, ip_len);
    }
    if (port)
    {
        *port = ntohs(cli_addr.sin_port);
    }
    return ret;
}

void read_proc(int fd, int mask)
{
    char buf[1024];
    int ret = recv(fd, buf, 1024, 0);
    if(ret == 0)
    {
        printf("client gone!\n");
        pReactor->removeEvent(fd, EVENT_READABLE | EVENT_WRITABLE);
    }
    else if(ret == -1)
    {
        printf("recv err\n");
    }
    else if(ret > 0)
    {
        printf("recv: %s\n", buf);
        send(fd, buf, ret, 0);
    }
}

void accept_proc(int fd, int mask)
{
    char ip[INET_ADDRSTRLEN];
    int port;
    int ret = server_accept(fd, ip, INET_ADDRSTRLEN, &port);
    if (ret != -1)
    {
        printf("client come here!%s: %d\n", ip, port);
        pReactor->registEvent(ret, EVENT_READABLE, read_proc);
    }
    else
    {
        printf("accept err\n");
    }
}

int main(int argc, char const *argv[])
{
    int serverfd = tnet::tcp_socket();
    int ret = tnet::tcp_listen(serverfd, 10086);
    pReactor = new Reactor;
    pReactor->registEvent(serverfd, EVENT_READABLE, accept_proc);
    printf("regist done!\n");
    pReactor->eventLoop();
    delete pReactor;
    return 0;
}
