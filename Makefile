CXX = g++
CXXFLAGS = -std=c++17 -Wall -I./src `pkg-config --cflags opencv4`
LDFLAGS = `pkg-config --libs opencv4`

SRC = $(wildcard src/*.cpp)
BIN = bin/compressor

all: $(BIN)

$(BIN): $(SRC)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $(SRC) -o $(BIN) $(LDFLAGS)


run: all
	./$(BIN)

clean:
	rm -rf bin/*
