
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "processpool.h"

class cgi_conn
{
private:
    static const int buff_size = 1024;
    static int m_epollfd;
    int m_socket;
    struct sockaddr_in m_address;
    char m_buff[buff_size];
    int m_read_idx;
public:
    cgi_conn(){}
    ~cgi_conn(){}

    void init(int epollfd, int sockfd, const sockaddr_in& cliaddr )
    {
        m_epollfd = epollfd;
        m_socket = sockfd;
        m_address = cliaddr;
        memset(m_buff, 0x00, buff_size);
        m_read_idx = 0;
    }

    void process()
    {
        int idx = 0;
        int ret = -1;

        while(true)
        {
            idx = m_read_idx;
            ret = recv(m_socket, m_buff + idx, buff_size - idx - 1, 0);

            printf("ret: %d  buff: %s\n", ret, m_buff);
            if(ret < 0)
            {
                if(errno != EINTR)
                {
                    removefd(m_epollfd, m_socket);
                }
                break;
            }
            else if(ret == 0)
            {
                removefd(m_epollfd, m_socket);
                break;
            }
            else
            {
                #if 0
                m_read_idx += ret;
                for(; idx < m_read_idx; ++idx)
                {
                    if((idx >= 1) && (m_buff[idx - 1] == '\r') && m_buff[idx] == '\n')
                    {
                        break;
                    }
                }

                printf("idx: %d  m_read_idx: %d\n", idx, m_read_idx);
                if(idx == m_read_idx)
                {
                    continue;
                }
                m_buff[idx - 1] = '\0';

                char* file_name = m_buff;
                if(access(file_name, F_OK) == -1)
                {
                    removefd(m_epollfd, m_socket);
                    break;
                }
                #endif

                ret = fork();
                if(ret == -1)
                {
                    removefd(m_epollfd, m_socket);
                    break;            
                }
                else if(ret > 0)
                {
                    removefd(m_epollfd, m_socket);
                    break;   
                }
                else
                {
                    //printf("command: %s\n", m_buff);
                    //close(STDOUT_FILENO);
                    //dup(m_socket);
                    //execl(m_buff, m_buff, NULL);
                    //exit(0);
                }
            }
        }
    }        
};

int cgi_conn::m_epollfd = -1;

int main(int argc, char* argv[])
{
    if(argc <= 2)
    {
        printf("usage: %s IP address port number\n", basename(argv[0]));
        return 1;
    }

    const char* ip = argv[1];
    int port = atoi(argv[2]);

    int listenfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(listenfd >= 0);

    int ret = 0;

    struct sockaddr_in sockaddr;
    bzero(&sockaddr, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &sockaddr.sin_addr);
    sockaddr.sin_port = htons(port);

    ret = bind(listenfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr));
    assert(ret != -1);

    ret = listen(listenfd, 5);
    assert(ret != -1);

    processpool<cgi_conn>* pool =  processpool < cgi_conn >::create(listenfd);
    if(pool)
    {
        pool->run();
        delete pool;
    }

    close(listenfd);
    return 0;
    
}






















































