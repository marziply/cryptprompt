CC = gcc
EXE = cryptprompt
BUILD_DIR = ./build
C_FLAGS = -std=c2x -c
DEV_C_FLAGS = -g -Wall
NCURSES_LIBS = -lncurses
SOURCES = $(wildcard src/*.c)
OBJECTS = $(patsubst src/%.c,build/%.o,$(SOURCES))

all: $(EXE)

run: C_FLAGS += $(DEV_C_FLAGS)
run: $(EXE)
	./$(EXE)

$(EXE): $(notdir $(OBJECTS))
	$(CC) $(OBJECTS) $(NCURSES_LIBS) -o $@

%.o: src/%.c
	@mkdir -p $(BUILD_DIR)

	$(CC) $(C_FLAGS) $< -o $(BUILD_DIR)/$@

clean:
	rm -rf $(BUILD_DIR)/*.o $(EXE)
