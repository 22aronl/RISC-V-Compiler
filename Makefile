
SRCS := $(shell find . -type f -name "*.cpp")
OBJS := $(SRCS:.cpp=.o)
CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++11 -O0

BTA_FILES := $(wildcard tests/icg/*.bta)

TARGET := parser_binary

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET) $(BTA_FILES)
	@echo "Running with input from each .bta file..."
	@for file in $(BTA_FILES); do \
		echo "Running with input from $$file..."; \
		./$(TARGET) < $$file; \
	done

# Clean rule to remove object files and the executable
clean:
	rm -f $(OBJS) $(TARGET)