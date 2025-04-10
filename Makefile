CXX = g++
CXXFLAGS = -std=c++17 -Wall -I./src `pkg-config --cflags opencv4` `pkg-config --cflags Magick++`
LDFLAGS = `pkg-config --libs opencv4` `pkg-config --libs Magick++`

SRC_DIR = src
BIN_DIR = bin
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
TARGET = $(BIN_DIR)/main

all: $(TARGET)

$(TARGET): $(SOURCES) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

run: $(TARGET)
	$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean