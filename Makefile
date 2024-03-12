
# Compiler
CPP = g++

# Compiler flags
CPP_FLAGS = -g -Wall -Wextra  -std=c++11

# Include directories
INCLUDES = -I./include -I/usr/include/GL -I./src/visuals


# Libraries
LIBS = -lGLEW -lGL -lglfw -lGLU

vpath %.cpp src src/visuals

# Source
SRCS = $(shell find src -name "*.cpp")

#
OBJS = $(patsubst %.cpp,%.o,$(SRCS))

# Name of the program
TARGET = main

# Default rule
all: $(TARGET) run clean

# Rule to compile .cpp files to object files
%.o: %.cpp
	$(CPP) $(CPP_FLAGS) $(INCLUDES) -c $< -o $@

# Rule to link object files into the executable
$(TARGET): $(OBJS)
	$(CPP) $(CPP_FLAGS) $(OBJS) -o $(TARGET) $(LIBS)

# Rule to run the program
run: $(TARGET)
	./$(TARGET)

# Clean rule
clean:
	rm -f $(OBJS) $(TARGET)