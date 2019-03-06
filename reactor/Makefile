CC = g++
CFLAGS = -std=c++11 -fPIC -g
CSHARED = -shared

test: test.o select_demultiplexer.o reactor.o tnet.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c*
	$(CC) $(CFLAGS) -c $<

.PHONY: clean

clean:
	rm -f *.o

