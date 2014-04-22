CC=g++-4.8

CFLAGS=-std=c++11 -I. -I/usr/local/Cellar/boost/1.55.0_1/include

LDIR= -L./ -L/usr/local/Cellar/boost/1.55.0_1/lib
LIBS= -lpthread

OBJ=cache.o

%.o: test/%.cpp
	$(CC) $(CFLAGS) -c $<

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

eviction_test: $(OBJ) eviction_test.o
	$(CC) $(CFLAGS) -o bin/eviction_test eviction_test.o $(OBJ) $(LIDR) $(LIBS)

basic_io_test: $(OBJ) basic_io_test.o
	$(CC) $(CFLAGS) -o bin/basic_io_test basic_io_test.o $(OBJ) $(LIDR) $(LIBS)

expiration_test: $(OBJ) expiration_test.o
	$(CC) $(CFLAGS) -o bin/expiration_test expiration_test.o $(OBJ) $(LIDR) $(LIBS)

all: expiration_test basic_io_test eviction_test

clean:
	rm -f *.o bin/basic_io_test bin/expiration_test
