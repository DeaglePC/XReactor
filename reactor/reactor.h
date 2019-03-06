#ifndef __REACTOR_H__
#define __REACTOR_H__

#include "event.h"
#include "event_demultiplexer.h"
#include "select_demultiplexer.h"
#include <map>

class Reactor
{
private:
    EventDemultiplexer *m_demultiplexer;
    EventHandlerMap m_fileEvents;
    FiredEvents m_firedEvents; // 多次结果用同一份内存
    bool m_isStopLoop;

public:
    Reactor();
    ~Reactor();

    void registEvent(int fd, int mask, FileProc proc);
    void removeEvent(int fd, int mask);
    void eventLoop();
};

#endif // __REACTOR_H__