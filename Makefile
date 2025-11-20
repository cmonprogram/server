CC = gcc
CFLAGS = -Wall
TARGET = server
TEST_TARGET = run_tests
DAEMON_NAME = server_daemon.service 

all : $(TARGET) $(TEST_TARGET)
$(TEST_TARGET):  CFLAGS += -DDISABLE_PRINT 

#link
$(TARGET) : main.o arg.o
	$(CC) $(CFLAGS) main.o arg.o -o $(TARGET)

#link
$(TEST_TARGET): test.o arg.o
	$(CC) $(CFLAGS) tests.o arg.o -o $(TEST_TARGET)

#not link
test.o: 
		$(CC) $(CFLAGS) -c tests.c  -o tests.o

#not link
main.o: 
		$(CC) $(CFLAGS) -c main.c  -o main.o

#not link
arg.o: arg.c 
		$(CC) $(CFLAGS) -c arg.c -o arg.o

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
