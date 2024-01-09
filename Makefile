CC = g++
CFLAGS = -c -Wall
DEBUG = 0
SRC_DIR_CONSUMER = consumer/src
SRC_DIR_PRODUCER = producer/src
BUILD_DIR_CONSUMER = consumer/build
BUILD_DIR_PRODUCER = producer/build
OBJECTS_CONSUMER = $(BUILD_DIR_CONSUMER)/Main.o $(BUILD_DIR_CONSUMER)/TcpServer.o $(BUILD_DIR_CONSUMER)/TcpConnection.o $(BUILD_DIR_CONSUMER)/RbufferManager.o $(BUILD_DIR_CONSUMER)/Statistics.o $(BUILD_DIR_CONSUMER)/InputManager.o
OBJECTS_PRODUCER = $(BUILD_DIR_PRODUCER)/Main.o $(BUILD_DIR_PRODUCER)/TcpClient.o $(BUILD_DIR_PRODUCER)/ValueManager.o $(BUILD_DIR_PRODUCER)/InputManager.o

.PHONY: all directories clean consumer clones

all: directories consumer producer clones

producer: $(OBJECTS_PRODUCER)
	$(CC) $(OBJECTS_PRODUCER) -o $(BUILD_DIR_PRODUCER)/Main

consumer: $(OBJECTS_CONSUMER)
	$(CC) $(OBJECTS_CONSUMER) -o $(BUILD_DIR_CONSUMER)/Main

clones:
	cp $(BUILD_DIR_CONSUMER)/Main consume 
	cp $(BUILD_DIR_PRODUCER)/Main produce

directories:
	mkdir -p $(BUILD_DIR_CONSUMER) $(BUILD_DIR_PRODUCER)

$(BUILD_DIR_CONSUMER)/%.o: $(SRC_DIR_CONSUMER)/%.cpp
	$(CC) $(CFLAGS) -DDEBUG=$(DEBUG) $< -o $@ 

$(BUILD_DIR_PRODUCER)/%.o: $(SRC_DIR_PRODUCER)/%.cpp
	$(CC) $(CFLAGS) -DDEBUG=$(DEBUG) $< -o $@ 

clean:
	rm -rf $(BUILD_DIR_CONSUMER) $(BUILD_DIR_PRODUCER) consume produce
