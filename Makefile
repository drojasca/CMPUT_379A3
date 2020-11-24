CFLAGS = -Wall -std=c++11
CC = g++

# https://stackoverflow.com/questions/30573481/path-include-and-src-directory-makefile/30602701
TARGET_SERVER = server
TARGET_CLIENT = client

SERVER_DIR = ./server_code
CLIENT_DIR = ./client_code

SOURCE_SERVER = $(wildcard $(SERVER_DIR)/*.cpp)
SOURCE_CLIENT = $(wildcard $(CLIENT_DIR)/*.cpp)

OBJECTS_SERVER = $(SOURCE_SERVER:$(SERVER_DIR)/%.cpp=$(SERVER_DIR)/%.o)
OBJECTS_CLIENT = $(SOURCE_CLIENT:$(CLIENT_DIR)/%.cpp=$(CLIENT_DIR)/%.o)

.PHONY: all clean

# optimize: CFLAGS += -O3
# optimize: all

# debug: CFLAGS += -g
# debug: all

all: $(TARGET_CLIENT) $(TARGET_SERVER)

$(TARGET_CLIENT): $(OBJECTS_CLIENT)
	$(CC) $(CFLAGS) $(OBJECTS_CLIENT) -o $(TARGET_CLIENT)

$(CLIENT_DIR)/%.o: $(CLIENT_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET_SERVER): $(OBJECTS_SERVER)
	$(CC) $(CFLAGS) $(OBJECTS_SERVER) -o $(TARGET_SERVER)

$(SERVER_DIR)/%.o: $(SERVER_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@


# depend gotten from https://stackoverflow.com/questions/2394609/makefile-header-dependencies
depend: .depend_server
depend: .depend_client


.depend_server: $(SOURCE_SERVER)
	rm -f ./.depend_server
	$(CC) $(CFLAGS) -MM $^ -MF  ./.depend_server

.depend_client: $(SOURCE_CLIENT)
	rm -f ./.depend_client
	$(CC) $(CFLAGS) -MM $^ -MF  ./.depend_client

include .depend_server
include .depend_client

clean:
	rm -f core client $(CLIENT_DIR)/*.o 
	rm -f core server $(SERVER_DIR)/*.o 

-include $(OBJ_SERVER:.o=.d)
-include $(OBJ_CLIENT:.o=.d)
