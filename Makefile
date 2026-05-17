CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I src
SRC = $(wildcard src/*.cpp)
TARGET = banking

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
