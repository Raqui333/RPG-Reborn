CXX = g++
CXXFLAGS = -std=c++17 -lSDL2 -lSDL2_image -lSDL2_ttf
SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)
BIN = rpg

all: $(BIN)

rpg: $(OBJ)
	$(CXX) $^ -o $@ $(CXXFLAGS)

.PHONY: clean

clean:
	@rm src/*.o 2> /dev/null
	@rm rpg 2> /dev/null
