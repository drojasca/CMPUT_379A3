CFLAGS = -Wall -std=c++11usr/share/man/man1/client_info.1
CC = g++

TARGET_SERVER = server
TARGET_CLIENT = client

SERVER_DIR = ./server_code
CLIENT_DIR = ./client_code
COMMON_DIR = ./common

SOURCE_SERVER = $(wildcard $(SERVER_DIR)/*.cpp)
SOURCE_CLIENT = $(wildcard $(CLIENT_DIR)/*.cpp)
SOURCE_COMMON = $(wildcard $(COMMON_DIR)/*.cpp)

OBJECTS_SERVER := $(SOURCE_SERVER:%.cpp=%.o)
OBJECTS_CLIENT := $(SOURCE_CLIENT:%.cpp=%.o)
OBJECTS_COMMON := $(SOURCE_COMMON:%.cpp=%.o)


# auto-generate header file deps gotten from: https://stackoverflow.com/questions/297514/how-can-i-have-a-makefile-automatically-rebuild-source-files-that-include-a-modif/2501673
DEPS_SERVER := $(OBJECTS_SERVER:.o=.d)
DEPS_CLIENT := $(OBJECTS_CLIENT:.o=.d)
DEPS_COMMON := $(OBJECTS_COMMON:.o=.d)


.PHONY: all clean

optimize: CFLAGS += -O3
optimize: all

debug: CFLAGS += -g
debug: all

all: $(TARGET_CLIENT) $(TARGET_SERVER)

$(TARGET_CLIENT): $(OBJECTS_CLIENT) $(OBJECTS_COMMON)
	$(CC) $(CFLAGS) $(OBJECTS_CLIENT) $(OBJECTS_COMMON) -o $(TARGET_CLIENT) 

$(CLIENT_DIR)/%.o: $(CLIENT_DIR)/%.cpp	
	$(CC) $(CFLAGS) -c -MMD -o $@ $<
	groff -Tpdf -man client.1 > client.pdf


$(TARGET_SERVER): $(OBJECTS_SERVER) $(OBJECTS_COMMON)
	$(CC) $(CFLAGS) $(OBJECTS_SERVER) $(OBJECTS_COMMON) -o $(TARGET_SERVER)


$(SERVER_DIR)/%.o:	$(SERVER_DIR)/%.cpp
	$(CC) $(CFLAGS) -c -MMD -o $@ $<


$(SOURCE_COMMON)/%.o:$(SOURCE_COMMON)/%.cpp
	$(CC) $(CFLAGS) -c -MMD -o $@ $<


clean:
	rm -f core $(COMMON_DIR)/*.o $(COMMON_DIR)/*.d
	rm -f core client $(CLIENT_DIR)/*.o $(CLIENT_DIR)/*.d 
	rm -f core server $(SERVER_DIR)/*.o $(SERVER_DIR)/*.d


-include $(DEPS_SERVER)
-include $(DEPS_CLIENT)
-include $(DEPS_COMMON)
