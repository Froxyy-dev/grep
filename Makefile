# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Iinclude

# Source directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build

# Output binary
TARGET = server

# Source files and object files
SRCS = $(SRC_DIR)/Matcher.cpp $(SRC_DIR)/Server.cpp $(SRC_DIR)/tokenizer.cpp $(SRC_DIR)/tokens.cpp
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Dependencies
DEPS = $(INCLUDE_DIR)/Matcher.hpp $(INCLUDE_DIR)/tokenizer.hpp $(INCLUDE_DIR)/tokens.hpp $(INCLUDE_DIR)/utils.hpp

# Default target
all: $(TARGET)

# Compile target
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

# Create the build directory and compile each object file
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Phony targets
.PHONY: all clean

