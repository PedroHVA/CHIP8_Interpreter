# Source files
CPP_FILES += $(wildcard src/*.cpp) $(wildcard src/chip8/*.cpp)

CPP_OBJECTS += $(addprefix objects/,$(notdir $(CPP_FILES:.cpp=.o)))

# Common flags

HEADERS += -Isrc/chip8 -Isrc

CFLAGS += -Wall -std=c++11

# Rules
build/chip8: $(CPP_OBJECTS)
	@test -d build || mkdir build
	g++ $^ $(LFLAGS) -o $@

objects/%.o: src/%.cpp
	@test -d objects || mkdir objects
	g++ $(CFLAGS) $(HEADERS) -c $< -o $@

objects/%.o: src/chip8/%.cpp
	@test -d objects || mkdir objects
	g++ $(CFLAGS) $(HEADERS) -c $< -o $@

-include $(CPP_OBJECTS:.o=.d)