FRAMEWORK = -framework AppKit -framework SkyLight
PRIVATE_FRAMEWORK = -iframework /System/Library/PrivateFrameworks
INCLUDE_FLAGS = -I./
BUILD_FLAGS = -Wall
SOURCES = $(wildcard ./source/*.c ./source/*.m)
MAIN = ./main.c
OUT_DIR = .
OUT_FILE = $(OUT_DIR)/mac_window

all: install

.PHONY: all

install:
	clang $(BUILD_FLAGS) -o $(OUT_FILE) $(MAIN) $(INCLUDE_FLAGS) $(SOURCES) $(PRIVATE_FRAMEWORK) $(FRAMEWORK)

testing:
	clang -o $(OUT_FILE) $(MAIN) $(INCLUDE_FLAGS) $(SOURCES) $(PRIVATE_FRAMEWORK) $(FRAMEWORK)

debug:
	clang $(BUILD_FLAGS) -g -o $(OUT_FILE) $(MAIN) $(INCLUDE_FLAGS) $(SOURCES) $(PRIVATE_FRAMEWORK) $(FRAMEWORK)

clean:
	rm -f $(OUT_FILE)
