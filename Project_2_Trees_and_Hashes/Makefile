# CXX = g++
# CXXFLAGS = -std=c++17 -g -Wall -O2 

# PROG ?= main

# # Core objects
# CORE_OBJS = \
# 	Item.o \
# 	Compare.o \
# 	ItemGenerator.o \

# # Main program objects
# MAIN_OBJS = main.o

# OBJS = $(MAIN_OBJS) $(CORE_OBJS) $(TEST_OBJS)

# mainprog: $(PROG)

# .cpp.o:
# 	$(CXX) $(CXXFLAGS) -c -o $@ $<

# $(PROG): $(OBJS)
# 	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

# clean:
# 	rm -rf $(PROG) *.o *.out \
# 		*.o \
# 		*/*.o 

# rebuild: clean main

CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -Wall -O2 -fno-elide-constructors
SRC = src/Item.cpp src/Compare.cpp src/Inventory.cpp src/HashInventory.cpp src/TreeInventory.cpp src/ItemAVL.cpp src/ItemGenerator.cpp main.cpp
OBJS = src/Item.o src/Compare.o src/Inventory.o src/HashInventory.o src/TreeInventory.o src/ItemAVL.o src/ItemGenerator.o main.o
TARGET = timing

.PHONY: clean rebuild

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)

rebuild: clean all