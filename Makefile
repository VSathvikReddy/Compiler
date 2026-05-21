#Needs fixing

TARGET_EXEC := compiler

BUILD_DIR := build
SRC_DIR := src
INC_DIR := src

SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INCS := $(shell find $(INC_DIR) -type d)
INC_FLAGS := $(addprefix -I,$(INCS))
# (C Preprocessor Flags)
CPPFLAGS := $(INC_FLAGS) -MMD -MP


# (External Libraries and Linker Flags)
LDFLAGS :=
# (C++ Compiler Flags)
Warning_FLAGS := -Wall -Wextra 
CXXFLAGS := -std=c++17 $(Warning_FLAGS)

CXX := g++

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


run: $(BUILD_DIR)/$(TARGET_EXEC)
	./$(BUILD_DIR)/$(TARGET_EXEC)


.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)



-include $(DEPS)
