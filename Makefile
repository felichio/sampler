CC = g++
CFLAGS = -c -Wall
SRC_DIR = consumer/src
BUILD_DIR = consumer/build
OBJECTS = $(BUILD_DIR)/Main.o $(BUILD_DIR)/TcpServer.o

.PHONY: all directories clean

all: directories $(OBJECTS)
	$(CC) $(OBJECTS) -o $(BUILD_DIR)/Main

directories:
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) $< -o $@ 

clean:
	rm -rf $(BUILD_DIR)
