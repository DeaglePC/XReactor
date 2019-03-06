#include "reactor.h"
#include <cstdio>

Reactor::Reactor() : m_demultiplexer(nullptr), m_isStopLoop(false)
{
    m_demultiplexer = new SelectDemultiplexer;
}

Reactor::~Reactor()
{
    if (m_demultiplexer != nullptr)
    {
        delete m_demultiplexer;
    }
}

void Reactor::registEvent(int fd, int mask, FileProc proc)
{
    auto it = m_fileEvents.find(fd);
    if (it == m_fileEvents.end())
    {
        FileEvent fe;
        fe.mask = mask;
        if (mask & EVENT_READABLE)
        {
            fe.rFileProc = proc;
        }
        if (mask & EVENT_WRITABLE)
        {
            fe.wFileProc = proc;
        }
        m_fileEvents[fd] = fe;
    }
    else
    {
        it->second.mask |= mask;
        if (mask & EVENT_READABLE)
        {
            it->second.rFileProc = proc;
        }
        if (mask & EVENT_WRITABLE)
        {
            it->second.wFileProc = proc;
        }
    }
    m_demultiplexer->addEvent(fd, mask);
}

void Reactor::removeEvent(int fd, int mask)
{
    auto it = m_fileEvents.find(fd);
    if (it == m_fileEvents.end())
    {
        return;
    }
    it->second.mask = it->second.mask & (~mask);
    m_demultiplexer->delEvent(fd, mask);
}

void Reactor::eventLoop()
{
    int ret;
    while (!m_isStopLoop)
    {
        printf("poll event!\n");
        ret = m_demultiplexer->pollEvent(m_fileEvents, m_firedEvents, NULL);
        if (ret == -1)
        {
            printf("eventpoll err!\n");
            break;
        }

        printf("poll event done! %d\n", ret);
        
        for(int i = 0; i < ret; i++)
        {
            int fd = m_firedEvents[i].fd;
            int mask = m_firedEvents[i].mask;
            if(mask & EVENT_READABLE)
            {
                m_fileEvents[fd].rFileProc(fd, mask);
            }
            if(mask & EVENT_WRITABLE)
            {
                m_fileEvents[fd].wFileProc(fd, mask);
            }
        }
    }
}
