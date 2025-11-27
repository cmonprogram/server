CC =  gcc #clang #gcc
CFLAGS = -Wall -g #-fsanitize=address,undefined  #-fsanitize=leak,thread  -Wextra -pedantic -O2 
TARGET = server
TEST_TARGET = run_tests
DAEMON_NAME = server_daemon.service 
OBJECTS = arg.o socket.o server.o instance.o cmd.o 

all : $(TARGET) $(TEST_TARGET)
$(TEST_TARGET):  CFLAGS += -DDISABLE_PRINT 

#link
$(TARGET) : main.o $(OBJECTS)
	$(CC) $(CFLAGS) main.o $(OBJECTS) -o $(TARGET)

#link
$(TEST_TARGET):  tests.o $(OBJECTS)
	$(CC) $(CFLAGS)  tests.o  $(OBJECTS) -o $(TEST_TARGET)

#not link
test.o: 
		$(CC) $(CFLAGS) -c tests.c  -o tests.o

#not link
main.o: 
		$(CC) $(CFLAGS) -c main.c  -o main.o

#not link
arg.o: arg.c 
		$(CC) $(CFLAGS) -c arg.c -o arg.o

#not link
socket.o: socket.c 
		$(CC) $(CFLAGS) -c socket.c -o socket.o

#not link
server.o: server.c 
		$(CC) $(CFLAGS) -c server.c -o server.o

#not link
instance.o: instance.c 
		$(CC) $(CFLAGS) -c instance.c -o instance.o

#not link
cmd.o: cmd.c 
		$(CC) $(CFLAGS) -c cmd.c -o cmd.o

install: all
	cp  $(TARGET) /usr/local/bin
	cp $(DAEMON_NAME) /etc/systemd/system/ 
	systemctl daemon-reload
	systemctl enable  $(DAEMON_NAME)
	systemctl start  $(DAEMON_NAME)

remove: 
	systemctl stop $(DAEMON_NAME)
	systemctl disable $(DAEMON_NAME)
	systemctl daemon-reload
	rm /usr/local/bin/$(TARGET) 
	rm /etc/systemd/system/$(DAEMON_NAME)
	rm -f $(TARGET)  $(TEST_TARGET) *.o

.PHONY: clean
clean:
	rm -f $(TARGET)  $(TEST_TARGET) *.o
