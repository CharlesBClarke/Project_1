CXX     = g++
CXXFLAGS= -Wall -g
TARGET  = build/h1_counter
SRC     = h1_counter.cpp

$(TARGET): $(SRC) | build
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

build:
	mkdir -p build
clean:
	rm -r build

.PHONY: clean
