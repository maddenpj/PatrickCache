CC=g++-4.8

CFLAGS=-std=c++11 -I. -I/usr/local/Cellar/boost/1.55.0_1/include

LDIR= -L./ -L/usr/local/Cellar/boost/1.55.0_1/lib
LIBS=

OBJ=main.o cache.o

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

cache: $(OBJ)
	$(CC) $(CFLAGS) -o cache_test $(OBJ) $(LDIR) $(LIBS)

all: cache 

clean:
	rm -f *.o cache_test 
