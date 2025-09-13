CXX     = g++
CXXFLAGS= -Wall -g
TARGET  = h1_counter
SRC     = h1_counter

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

.PHONY: clean
