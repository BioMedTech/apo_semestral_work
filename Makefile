CC = arm-linux-gnueabihf-gcc
CXX = arm-linux-gnueabihf-g++

CPPFLAGS = -I .
CFLAGS =-g -std=gnu99 -O1 -Wall
CXXFLAGS = -g -std=gnu++11 -O1 -Wall
LDFLAGS = -lrt -lpthread -luuid

SRC_EXT = c
SRC_PATH = src
BUILD_PATH = build
BIN_PATH = $(BUILD_PATH)/bin

SOURCES = $(shell find $(SRC_PATH) -name '*.$(SRC_EXT)' | sort -k 1nr | cut -f2-)
#SOURCES += font_prop14x16.c font_rom8x16.c
TARGET_EXE = main
#TARGET_IP ?= 192.168.202.127
ifeq ($(TARGET_IP)$(filter run,$(MAKECMDGOALS)),run)
$(warning The target IP address is not set)
$(warning Run as "TARGET_IP=192.168.202.203 make run" or modify Makefile)
TARGET_IP?=192.168.202.203
endif
TARGET_IP?=192.168.202.203
TARGET_DIR?=/tmp/$(shell whoami)
TARGET_USER?=root
# for use from Eduroam network use TARGET_IP=localhost and enable next line
# SSH_OPTIONS=-o "UserKnownHostsFile=/dev/ttyUSB0" -o "StrictHostKeyChecking=no" -o "Port=115200"
SSH_OPTIONS=-i /opt/zynq/ssh-connect/mzapo-root-key
# ssh -i /opt/zynq/ssh-connect/mzapo-root-key root@192.168.202.211


# path #
SRC_PATH = src
BUILD_PATH = build
BIN_PATH = $(BUILD_PATH)/bin

# executable # 
BIN_NAME = runner

# extensions #
SRC_EXT = c

# code lists #
# Find all source files in the source directory, sorted by
# most recently modified
SOURCES = $(shell find $(SRC_PATH) -name '*.$(SRC_EXT)' | sort -k 1nr | cut -f2-)
# Set the object file names, with the source directory stripped
# from the path, and the build path prepended in its place
OBJECTS = $(SOURCES:$(SRC_PATH)/%.$(SRC_EXT)=$(BUILD_PATH)/%.o)
# Set the dependency files that will be used to add header dependencies
DEPS = $(OBJECTS:.o=.d)

# flags #
COMPILE_FLAGS = -std=gnu99 -Wall -Wextra -g -pthread
INCLUDES = -I include/ -I /usr/local/include
LDFLAGS = -lrt -pthread
# Space-separated pkg-config libraries used by this project
LIBS =

.PHONY: default_target
default_target: release

.PHONY: release
release: export CCFLAGS := $(CCFLAGS) $(COMPILE_FLAGS)
release: dirs
	@$(MAKE) all

.PHONY: dirs
dirs:
	@echo "Creating directories"
	@mkdir -p $(dir $(OBJECTS))
	@mkdir -p $(BIN_PATH)

.PHONY: clean
clean:
	@echo "Deleting $(BIN_NAME) symlink"
	@$(RM) $(BIN_NAME)
	@echo "Deleting directories"
	@$(RM) -r $(BUILD_PATH)
	@$(RM) -r $(BIN_PATH)

# checks the executable and symlinks to the output
.PHONY: all
all: $(BIN_PATH)/$(BIN_NAME)
	@echo "Making symlink: $(BIN_NAME) -> $<"
	@$(RM) $(BIN_NAME)
	@ln -s $(BIN_PATH)/$(BIN_NAME) $(BIN_NAME)

# Creation of the executable
$(BIN_PATH)/$(BIN_NAME): $(OBJECTS)
	@echo "Linking: $@"
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

# Add dependency files, if they exist
-include $(DEPS)

# Source file rules
# After the first compilation they will be joined with the rules from the
# dependency files to provide header dependencies
$(BUILD_PATH)/%.o: $(SRC_PATH)/%.$(SRC_EXT)
	@echo "Compiling: $< -> $@"
	$(CC) $(CCFLAGS) $(INCLUDES) -MP -MMD -c $< -o $@


connect:
	ssh $(SSH_OPTIONS) $(TARGET_USER)@$(TARGET_IP)


copy-executable:
	ssh $(SSH_OPTIONS) -t $(TARGET_USER)@$(TARGET_IP) killall gdbserver 1>/dev/null 2>/dev/null || true
	ssh $(SSH_OPTIONS) $(TARGET_USER)@$(TARGET_IP) mkdir -p $(TARGET_DIR)
	scp $(SSH_OPTIONS) $(BIN_PATH)/$(BIN_NAME) index.bmp $(TARGET_USER)@$(TARGET_IP):$(TARGET_DIR)/
.PHONY: copy-executable

run: copy-executable $(BIN_PATH)/$(BIN_NAME)
	ssh $(SSH_OPTIONS) -t $(TARGET_USER)@$(TARGET_IP) "cd $(TARGET_DIR) && ./$(BIN_NAME)"
.PHONY: run

debug: copy-executable $(BIN_PATH)/$(BIN_NAME)
	xterm -e ssh $(SSH_OPTIONS) -t $(TARGET_USER)@$(TARGET_IP) gdbserver :12345 $(TARGET_DIR)/$(BIN_NAME) &
	sleep 2
	echo >connect.gdb "target extended-remote $(TARGET_IP):12345"
	echo >>connect.gdb "b main"
	echo >>connect.gdb "c"
	ddd --debugger gdb-multiarch -x connect.gdb $(BIN_PATH)/$(BIN_NAME)
.PHONY: debug

-include depend
