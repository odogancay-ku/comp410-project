
# Compiler
CPP = g++

# Compiler flags
CPP_FLAGS = -g -w  -std=c++11

# Include directories
INCLUDES = -I./include -I./src/visuals -I/usr/include/GL


# Libraries
LIBS = -lGLEW -lGL -lglfw -lGLU

vpath %.cpp src src/visuals

# Source
SRCS = $(shell find src -name "*.cpp")


OBJS = $(patsubst %.cpp,%.o,$(SRCS))

# Name of the program
TARGET = main

# Default rule
all: clean $(TARGET) run clean

all-nvidia: clean $(TARGET) run-nvidia clean

# Rule to compile .cpp files to object files
%.o: %.cpp
	$(CPP) $(CPP_FLAGS) $(INCLUDES) -c $< -o $@

# Rule to link object files into the executable
$(TARGET): $(OBJS)
	$(CPP) $(CPP_FLAGS) $(OBJS) -o $(TARGET) $(LIBS)

# Rule to run the program
run: $(TARGET)
	./$(TARGET)

run-nvidia: $(TARGET)
	__NV_PRIME_RENDER_OFFLOAD=1 __GLX_VENDOR_LIBRARY_NAME=nvidia ./main

# Clean rule
clean:
	rm -f $(OBJS) $(TARGET)