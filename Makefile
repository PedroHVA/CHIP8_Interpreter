# Source files
CPP_FILES += $(wildcard src/*.cpp) $(wildcard src/chip8/*.cpp)

CPP_OBJECTS += $(addprefix objects/,$(notdir $(CPP_FILES:.cpp=.o)))

# Common flags
HEADERS += -Isrc/chip8 -Isrc
CFLAGS += -Wall -std=c++11 

# SDL
LFLAGS += -lmingw32 -lSDL2main -lSDL2 -Llib/SDL2-2.0.8/i686-w64-mingw32/lib
HEADERS += -Ilib/SDL2-2.0.8/i686-w64-mingw32/include/SDL2

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