# Directories
GCC_ROOT_DIR = /usr
GCC_BIN_DIR  = $(GCC_ROOT_DIR)/bin
GCC_INCLUDE_DIR = $(GCC_ROOT_DIR)/include
INCLUDE_DIRS = $(GCC_INCLUDE_DIR)
LIB_DIRS = $(GCC_ROOT_DIR)/lib

BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin

# Toolchain
CC = $(GCC_BIN_DIR)/gcc

# Files
TARGET = $(BIN_DIR)/tragic
SOURCES = main.c \
					gamelib.c \
					utils.c

#substitution references
OBJECT_NAMES =$(SOURCES:.c=.o)
OBJECTS = $(patsubst %, $(OBJ_DIR)/%, $(OBJECT_NAMES))

# Flags
WFLAGS = -Wall -Wextra -Werror -Wshadow
CFLAGS = $(WFLAGS) $(addprefix -I,$(INCLUDE_DIRS)) -Og -g
LDFLAGS = $(addprefix -L,$(LIB_DIRS))

# Linking
#automatic variables :
#  $@    :   $^
$(TARGET): $(OBJECTS)
	@mkdir -p $(dir $@) #manage build directories creation
	$(CC) $(LDFLAGS) $^ -o $@

#Compiling
$(OBJ_DIR)/%.o: %.c
		@mkdir -p $(dir $@)
		$(CC) $(CFLAGS) -c -o $@ $^

#Phonies
.PHONY: all clean

all: $(TARGET)

clean:
		$(RM) -r $(BUILD_DIR)
