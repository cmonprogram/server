CC = gcc
CFLAGS = -Wall
TARGET = server

all : TARGET

#link
TARGET : main.o arg.o
	$(CC) $(CFLAGS) main.o arg.o -o $(TARGET)

#not link
main.o: 
		$(CC) $(CFLAGS) -c main.c  -o main.o

#not link
arg.o: arg.c 
		$(CC) $(CFLAGS) -c arg.c -o arg.o

.PHONY: clean
clean:
	rm -f $(TARGET) *.o
