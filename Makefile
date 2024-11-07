CC = g++
CFLAGS = -c -Wall
DEBUG = 0
SRC_DIR_SAMPLER = sampler/src
SRC_DIR_PRODUCER = producer/src
SRC_DIR_CONSUMER = consumer/src
BUILD_DIR_SAMPLER = sampler/build
BUILD_DIR_PRODUCER = producer/build
BUILD_DIR_CONSUMER = consumer/build
OBJECTS_SAMPLER = $(BUILD_DIR_SAMPLER)/Main.o $(BUILD_DIR_SAMPLER)/TcpServer.o $(BUILD_DIR_SAMPLER)/TcpConnection.o $(BUILD_DIR_SAMPLER)/RbufferManager.o $(BUILD_DIR_SAMPLER)/Statistics.o $(BUILD_DIR_SAMPLER)/InputManager.o
OBJECTS_PRODUCER = $(BUILD_DIR_PRODUCER)/Main.o $(BUILD_DIR_PRODUCER)/TcpClient.o $(BUILD_DIR_PRODUCER)/ValueManager.o $(BUILD_DIR_PRODUCER)/InputManager.o
OBJECTS_CONSUMER = $(BUILD_DIR_CONSUMER)/Main.o $(BUILD_DIR_CONSUMER)/Handler.o $(BUILD_DIR_CONSUMER)/Reader.o $(BUILD_DIR_CONSUMER)/OutputStream.o $(BUILD_DIR_CONSUMER)/InputManager.o

.PHONY: all directories clean sampler producer consumer clones

all: directories sampler producer consumer clones

producer: $(OBJECTS_PRODUCER)
	$(CC) $(OBJECTS_PRODUCER) -o $(BUILD_DIR_PRODUCER)/Main

sampler: $(OBJECTS_SAMPLER)
	$(CC) $(OBJECTS_SAMPLER) -o $(BUILD_DIR_SAMPLER)/Main

consumer: $(OBJECTS_CONSUMER)
	$(CC) $(OBJECTS_CONSUMER) -o $(BUILD_DIR_CONSUMER)/Main

clones:
	cp $(BUILD_DIR_SAMPLER)/Main sample 
	cp $(BUILD_DIR_PRODUCER)/Main produce
	cp $(BUILD_DIR_CONSUMER)/Main consume

directories:
	mkdir -p $(BUILD_DIR_SAMPLER) $(BUILD_DIR_PRODUCER) $(BUILD_DIR_CONSUMER)

$(BUILD_DIR_SAMPLER)/%.o: $(SRC_DIR_SAMPLER)/%.cpp
	$(CC) $(CFLAGS) -DDEBUG=$(DEBUG) $< -o $@ 

$(BUILD_DIR_PRODUCER)/%.o: $(SRC_DIR_PRODUCER)/%.cpp
	$(CC) $(CFLAGS) -DDEBUG=$(DEBUG) $< -o $@

$(BUILD_DIR_CONSUMER)/%.o: $(SRC_DIR_CONSUMER)/%.cpp
	$(CC) $(CFLAGS) -DDEBUG=$(DEBUG) $< -o $@ 

clean:
	rm -rf $(BUILD_DIR_SAMPLER) $(BUILD_DIR_PRODUCER) $(BUILD_DIR_CONSUMER) sample produce consume
