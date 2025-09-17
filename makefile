CXX        = g++
PRIMEFLAGS = -Wall -O3 -march=native -flto -DNDEBUG
PERFFLAGS = -O3 -march=native -DNDEBUG -g -fno-omit-frame-pointer
BUGFLAGS   = -Wall -g

PRIMETARGET = h1-counter
PERFTARGET  = build/perf_h1-counter
BUGTARGET   = build/bug_h1-counter

SRC = h1-counter.cpp SocketStream.cpp

all: $(PRIMETARGET)

$(PRIMETARGET): $(SRC)
	$(CXX) $(PRIMEFLAGS) -o $@ $(SRC)

perf: build
	$(CXX) $(PERFFLAGS) -o $(PERFTARGET) $(SRC)

bug: build
	$(CXX) $(BUGFLAGS) -o $(BUGTARGET) $(SRC)

build:
	mkdir -p build

clean:
	rm -rf build $(PRIMETARGET)
