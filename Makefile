CFLAGS = -Wall -std=c++11
CC = g++
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:%.cpp=%.o)
TARGET = server

.PHONY: all clean

optimize: CFLAGS += -O3
optimize: all

debug: CFLAGS += -g
debug: all

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# depend gotten from https://stackoverflow.com/questions/2394609/makefile-header-dependencies
depend: .depend

.depend: $(SOURCES)
	rm -f ./.depend
	$(CC) $(CFLAGS) -MM $^ -MF  ./.depend

include .depend

clean:
	rm -f core *.o server
