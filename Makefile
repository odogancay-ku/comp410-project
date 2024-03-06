# Compiler
CPP = g++

# Compiler flags
CPP_FLAGS = -g -Wall -Wextra  -std=c++11

# Include directories
INCLUDES = -I/home/ofaru/COMP410/include -I/usr/include/GL


# Libraries
LIBS = -lGLEW -lGL -lglfw -lGLU

# Source
SRCS = main.cpp

#
OBJS = $(SRCS:.cpp=.o)

# Name of the program
TARGET = main

# Default rule
all: $(TARGET) run

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