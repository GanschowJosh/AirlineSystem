# Compiler and Flags
CXX = g++
CXXFLAGS = -std=c++17 -Iinclude

# Directories
SRCDIR = src
OBJDIR = obj
BINDIR = bin

# Files
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))
TARGET = $(BINDIR)/airline_app

# Default target
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile source files into object files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Create necessary directories if they do not exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

# Clean up build artifacts
clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all clean
