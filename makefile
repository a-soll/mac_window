FRAMEWORK = -framework AppKit -framework SkyLight
PRIVATE_FRAMEWORK = -iframework /System/Library/PrivateFrameworks
BUILD_FLAGS = -I./
SOURCES = $(wildcard ./source/*.c ./source/*.m)
MAIN = ./main.c
OUT_DIR = .
OUT_FILE = $(OUT_DIR)/a.out

all: install

.PHONY: all

install:
	clang -o $(OUT_FILE) $(MAIN) $(BUILD_FLAGS) $(SOURCES) $(PRIVATE_FRAMEWORK) $(FRAMEWORK)

clean:
	rm -f $(OUT_FILE)
