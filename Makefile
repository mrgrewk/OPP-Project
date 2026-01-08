# Compiler settings - Can be customized.
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g

# Project Name (The executable file)
TARGET = oopproj_2025

# Source files (Wildcard matches all .cpp files in directory)
SRCS = $(wildcard *.cpp)

# Object files (Replaces .cpp with .o)
OBJS = $(SRCS:.cpp=.o)

# Default rule: "make" runs this
all: $(TARGET)

# Rule to link the program
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule: "make clean" deletes build files
clean:
	rm -f $(OBJS) $(TARGET)