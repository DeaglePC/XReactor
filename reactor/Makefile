CC = g++
CFLAGS = -std=c++11 -fPIC -g
CSHARED = -shared

test: test.o select_demultiplexer.o reactor.o tnet.o timer_pool.o epoll_demultiplexer.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c*
	$(CC) $(CFLAGS) -c $<

.PHONY: clean

clean:
	rm -f *.o

.PHONY: test_timer

test_timer: test_timer.o timer_pool.o
	$(CC) $(CFLAGS) -o $@ $^

libreactor.so: reactor.o timer_pool.o select_demultiplexer.o epoll_demultiplexer.o
	$(CC) $(CFLAGS) $(CSHARED) -o $@ $^

.PHONY: shared
shared: libreactor.so
