CC=g++-4.8

CFLAGS=-std=c++11 -I. -I/usr/local/Cellar/boost/1.55.0_1/include

LDIR= -L./ -L/usr/local/Cellar/boost/1.55.0_1/lib
LIBS= -lpthread

OBJ=cache.o

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

basic_io_test: $(OBJ) basic_io_test.o
	$(CC) $(CFLAGS) -o basic_io_test basic_io_test.o $(OBJ) $(LIDR) $(LIBS)

expiration_test: $(OBJ) expiration_test.o
	$(CC) $(CFLAGS) -o expiration_test expiration_test.o $(OBJ) $(LIDR) $(LIBS)

all: expiration_test basic_io_test 

clean:
	rm -f *.o basic_io_test expiration_test
