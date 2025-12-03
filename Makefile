CC = g++
CFLAGS = -std=c++17 -O2 -Iinclude -Wall -Wextra
SRCDIR = src
OBJDIR = build
BINDIR = bin
SOURCES = $(SRCDIR)/main.cpp $(SRCDIR)/Graph.cpp
OBJS = $(OBJDIR)/main.o $(OBJDIR)/Graph.o
TARGET = $(BINDIR)/main

.PHONY: all run clean

all: $(TARGET)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS) | $(BINDIR)
	$(CC) $(CFLAGS) $^ -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(OBJDIR) $(BINDIR)
