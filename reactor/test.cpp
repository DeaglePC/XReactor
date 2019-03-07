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
    if (ret == 0)
    {
        printf("client gone!\n");
        pReactor->removeFileEvent(fd, EVENT_READABLE | EVENT_WRITABLE);
    }
    else if (ret == -1)
    {
        printf("recv err\n");
    }
    else if (ret > 0)
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
        pReactor->registFileEvent(ret, EVENT_READABLE, read_proc);
    }
    else
    {
        printf("accept err\n");
    }
}

long long id1, id2, id3;
int time_proc2(long long id);
int time_proc1(long long id)
{
    static int cnt = 0;
    printf("#wdnmd!: -%d- \n", ++cnt);
    if (cnt == 10)
    {
        id2 = pReactor->registTimeEvent(1000, time_proc2);
    }
    if (cnt == 15)
    {
        int ret = pReactor->removeTimeEvent(id2);
        if (ret != -1)
        {
            printf("#deleted: \n");
        }
        else
        {
            printf("#delete err not found id\n");
        }
        
    }
    if (cnt == 20)
    {
        return -1;
    }
    return 1000;
}

int time_proc2(long long id)
{
    printf("#qzzzzz!\n");
    return 1000;
}

int time_proc3(long long id)
{
    printf("#hello world\n");
    return 2000;
}

int main(int argc, char const *argv[])
{
    int serverfd = tnet::tcp_socket();
    int ret = tnet::tcp_listen(serverfd, 10086);
    pReactor = new Reactor;
    pReactor->registFileEvent(serverfd, EVENT_READABLE, accept_proc);
    id1 = pReactor->registTimeEvent(1000, time_proc1);
    id3 = pReactor->registTimeEvent(2000, time_proc3);
    printf("regist done!\n");
    pReactor->eventLoop(EVENT_LOOP_FILE_EVENT | EVENT_LOOP_TIMER_EVENT);
    delete pReactor;
    return 0;
}
