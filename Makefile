CC = g++
CFLAGS = -c -Wall
DEBUG = 0
SRC_DIR_SAMPLER = sampler/src
SRC_DIR_PRODUCER = producer/src
BUILD_DIR_SAMPLER = sampler/build
BUILD_DIR_PRODUCER = producer/build
OBJECTS_SAMPLER = $(BUILD_DIR_SAMPLER)/Main.o $(BUILD_DIR_SAMPLER)/TcpServer.o $(BUILD_DIR_SAMPLER)/TcpConnection.o $(BUILD_DIR_SAMPLER)/RbufferManager.o $(BUILD_DIR_SAMPLER)/Statistics.o $(BUILD_DIR_SAMPLER)/InputManager.o
OBJECTS_PRODUCER = $(BUILD_DIR_PRODUCER)/Main.o $(BUILD_DIR_PRODUCER)/TcpClient.o $(BUILD_DIR_PRODUCER)/ValueManager.o $(BUILD_DIR_PRODUCER)/InputManager.o

.PHONY: all directories clean sampler clones

all: directories sampler producer clones

producer: $(OBJECTS_PRODUCER)
	$(CC) $(OBJECTS_PRODUCER) -o $(BUILD_DIR_PRODUCER)/Main

sampler: $(OBJECTS_SAMPLER)
	$(CC) $(OBJECTS_SAMPLER) -o $(BUILD_DIR_SAMPLER)/Main

clones:
	cp $(BUILD_DIR_SAMPLER)/Main sample 
	cp $(BUILD_DIR_PRODUCER)/Main produce

directories:
	mkdir -p $(BUILD_DIR_SAMPLER) $(BUILD_DIR_PRODUCER)

$(BUILD_DIR_SAMPLER)/%.o: $(SRC_DIR_SAMPLER)/%.cpp
	$(CC) $(CFLAGS) -DDEBUG=$(DEBUG) $< -o $@ 

$(BUILD_DIR_PRODUCER)/%.o: $(SRC_DIR_PRODUCER)/%.cpp
	$(CC) $(CFLAGS) -DDEBUG=$(DEBUG) $< -o $@ 

clean:
	rm -rf $(BUILD_DIR_SAMPLER) $(BUILD_DIR_PRODUCER) sample produce
