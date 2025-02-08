# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++14 -I../include

# Directories
SRC_DIR = ..
CORE_DIR = $(SRC_DIR)/core
INCLUDE_DIR = $(SRC_DIR)/include
TEST_DIR = .

# Source files
CORE_SOURCES = $(CORE_DIR)/imageReader/imageFunc.cpp \
               $(CORE_DIR)/ntfs/attrProc.cpp \
               $(CORE_DIR)/ntfs/clusterProc.cpp \
               $(CORE_DIR)/ntfs/entryProc.cpp

TEST_SOURCES = $(TEST_DIR)/test_cluster_info.cpp

# Object files
CORE_OBJECTS = $(CORE_SOURCES:.cpp=.o)
TEST_OBJECTS = $(TEST_SOURCES:.cpp=.o)

# Target executable
TARGET = test

# Default target
all: $(TARGET)

# Linking the executable
$(TARGET): $(CORE_OBJECTS) $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target
clean:
	rm -f $(CORE_OBJECTS) $(TEST_OBJECTS) $(TARGET)

# Phony targets
.PHONY: all clean