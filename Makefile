# Farm Carbon Footprint Estimator Makefile
# Compatible with GCC/Clang on Linux/macOS

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
TARGET = carbon
SRCDIR = src
SOURCES = $(SRCDIR)/main.c $(SRCDIR)/input.c $(SRCDIR)/compute.c $(SRCDIR)/report.c $(SRCDIR)/ui.c $(SRCDIR)/simple_ui.c
OBJECTS = $(SOURCES:.c=.o)

# Default target - build unified version
all: $(TARGET)

# Build the unified executable with all interfaces
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) -lm

# Compile source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJECTS) $(TARGET) report.txt

# Install (copy to /usr/local/bin)
install: $(TARGET)
	sudo cp $(TARGET) /usr/local/bin/

# Uninstall
uninstall:
	sudo rm -f /usr/local/bin/$(TARGET)

# Run demo with sample data
demo: $(TARGET)
	./$(TARGET) data/sample_input.csv

# Run UI demo
demo-ui: $(TARGET)
	./$(TARGET) --ui

# Run simple UI demo
demo-simple: $(TARGET)
	./$(TARGET) --simple

# Debug build
debug: CFLAGS += -g -DDEBUG
debug: $(TARGET)

# Help
help:
	@echo "Available targets:"
	@echo "  all              - Build unified version with all interfaces (default)"
	@echo "  carbon           - Build unified version"
	@echo "  clean            - Remove build files"
	@echo "  install          - Install to /usr/local/bin"
	@echo "  uninstall        - Remove from /usr/local/bin"
	@echo "  demo             - Run CLI version with sample data"
	@echo "  demo-ui          - Run advanced UI version"
	@echo "  demo-simple      - Run simple UI version"
	@echo "  debug            - Build with debug symbols"
	@echo "  help             - Show this help"

.PHONY: all clean install uninstall demo debug help