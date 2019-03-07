#ifndef __REACTOR_H__
#define __REACTOR_H__

#include "event.h"
#include "event_demultiplexer.h"
#include "select_demultiplexer.h"
#include "timer_pool.h"
#include <map>

#define EVENT_LOOP_DONT_WAIT 1

class Reactor
{
private:
  EventDemultiplexer *m_demultiplexer;
  EventHandlerMap m_fileEvents;
  FiredEvents m_firedEvents; // 多次结果用同一份内存
  TimerPool m_timePool;      // 定时器

  bool m_isStopLoop;

  int processEvents(int flag);

public:
  Reactor();
  ~Reactor();

  void eventLoop();

  void registFileEvent(int fd, int mask, FileProc proc);
  void removeFileEvent(int fd, int mask);

  long long registTimeEvent(long long milliseconds, TimeProc timeProc);
  int removeTimeEvent(long long id);
};

#endif // __REACTOR_H__