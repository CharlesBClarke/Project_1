CXX     = g++
CXXFLAGS= -Wall -g
TARGET  = h1-counter
SRC      = h1-counter.cpp SocketStream.cpp

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -r build
