# File: Makefile
#  Student: Natacha Sousa, 107413
#  Description: Automates the compilation process of all .c source files 
#               into an executable.

# Compilation flags
CFLAGS = -O3 -Wall -Wextra -Werror -Wno-unused-result

# Executable name
TARGET = proj 

# Automatically find all .c source files
SRC = $(wildcard *.c)

# Replace .c extensions with .o to generate object files
OBJ = $(SRC:.c=.o)

# Default rule
all: $(TARGET)

# How to build the final executable from object files
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# How to build object files from .c source files
%.o: %.c
	$(CC) $(CFLAGS) -c $<

# Command to clean compiled files
clean:
	rm -f $(TARGET) $(OBJ)

# Prevents conflicts with files of the same name by declaring targets as PHONY
.PHONY: all clean
