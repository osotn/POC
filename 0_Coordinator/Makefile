#   _^_   Smart_Home project
#  /|o|\      (c) 2015
#  =====

CC=gcc
TARGET=coordinator
#CFLAGS=-g -Wall -Werror -o0
CFLAGS=-g -o0
HEADERS=coordinator.h server_interface.h slave_interface.h server_data.h \
  slave_data.h data.h
OBJECTS=main.o coordinator.o \
  server/interface_stub.o server/data_stub.o \
  slave/interface_stub.o slave/data_stub.o

all: $(TARGET)

$(TARGET): $(OBJECTS) $(HEADER)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

clean:
	rm -rf *.o servet/*.o slave/*.o $(TARGET) core
