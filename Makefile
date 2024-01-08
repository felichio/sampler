CC = g++
CFLAGS = -c -Wall
SRC_DIR_CONSUMER = consumer/src
SRC_DIR_PRODUCER = producer/src
BUILD_DIR_CONSUMER = consumer/build
BUILD_DIR_PRODUCER = producer/build
OBJECTS_CONSUMER = $(BUILD_DIR_CONSUMER)/Main.o $(BUILD_DIR_CONSUMER)/TcpServer.o $(BUILD_DIR_CONSUMER)/TcpConnection.o $(BUILD_DIR_CONSUMER)/RbufferManager.o $(BUILD_DIR_CONSUMER)/Statistics.o
OBJECTS_PRODUCER = $(BUILD_DIR_PRODUCER)/Main.o $(BUILD_DIR_PRODUCER)/TcpClient.o $(BUILD_DIR_PRODUCER)/ValueManager.o

.PHONY: all directories clean consumer

all: directories consumer producer

producer: $(OBJECTS_PRODUCER)
	$(CC) $(OBJECTS_PRODUCER) -o $(BUILD_DIR_PRODUCER)/Main

consumer: $(OBJECTS_CONSUMER)
	$(CC) $(OBJECTS_CONSUMER) -o $(BUILD_DIR_CONSUMER)/Main

directories:
	mkdir -p $(BUILD_DIR_CONSUMER) $(BUILD_DIR_PRODUCER)

$(BUILD_DIR_CONSUMER)/%.o: $(SRC_DIR_CONSUMER)/%.cpp
	$(CC) $(CFLAGS) -DDEBUG=$(DEBUG) $< -o $@ 

$(BUILD_DIR_PRODUCER)/%.o: $(SRC_DIR_PRODUCER)/%.cpp
	$(CC) $(CFLAGS) -DDEBUG=$(DEBUG) $< -o $@ 

clean:
	rm -rf $(BUILD_DIR_CONSUMER) $(BUILD_DIR_PRODUCER)
