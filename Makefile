CC = gcc
CFLAGS = -O2 -Wall -Wextra -std=c99 -Iinclude
SRC_DIR = src
SRC = $(wildcard $(SRC_DIR)/*.c)
OUT = game_of_life.out

INPUT_DIR = InputData
OUTPUT_DIR = OutputData

INPUT_FILES = $(wildcard $(INPUT_DIR)/data*.in)
FILE_ARGS = $(foreach infile,$(INPUT_FILES),$(infile) $(patsubst $(INPUT_DIR)/%.in,$(OUTPUT_DIR)/%.out,$(infile)))

.PHONY: all run clean

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

run: $(OUT) | $(OUTPUT_DIR)
	@echo "Generating output..."
	@./$(OUT) $(FILE_ARGS)
	@echo "Executed."

$(OUTPUT_DIR):
	@mkdir -p $(OUTPUT_DIR)

clean:
	@echo "Cleaning..."
	@rm -f $(OUT)
	@rm -rf $(OUTPUT_DIR)
