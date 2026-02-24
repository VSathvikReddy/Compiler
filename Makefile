TARGET_EXEC := compiler
TARGET_TO_COMPILE:= test.c
COMPILE_INTO:= a.out
ASSEMBLY_MID := a.asm

BUILD_DIR := build
SRC_DIR := src
INC_DIR := src 

SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

CC  := gcc

INCS := $(shell find $(INC_DIR) -type d)
INC_FLAGS := $(addprefix -I,$(INCS))
CPPFLAGS := $(INC_FLAGS) -MMD -MP 


#linkin step
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	mkdir -p $(BUILD_DIR)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# .o step
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) -c $< -o $@

.PHONY: clean run

run: $(BUILD_DIR)/$(TARGET_EXEC)
	./$(BUILD_DIR)/$(TARGET_EXEC) $(TARGET_TO_COMPILE)


clean:
	rm -rf $(BUILD_DIR)
	rm -f $(ASSEMBLY_MID) $(COMPILE_INTO)


-include $(DEPS)