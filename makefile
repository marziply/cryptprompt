CC = gcc
EXE = cryptprompt
BUILD_DIR = ./build
INSTALL_DIR = /usr/bin
INITCPIO_DIR = /etc/initcpio
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

install: uninstall $(EXE)
	@cp ./$(EXE) $(INSTALL_DIR)/$(EXE)
	@cp ./sys/hooks/$(EXE)_*_hook.sh $(INITCPIO_DIR)/hooks
	@cp ./sys/hooks/$(EXE)_*_install.sh $(INITCPIO_DIR)/install
	@cp ./sys/services/$(EXE).service /etc/systemd/system

uninstall:
	@rm -rf $(INSTALL_DIR)/$(EXE)
	@rm -rf $(INITCPIO_DIR)/{hooks,install}/$(EXE)_*.sh
	@rm -rf /etc/systemd/system/$(EXE).service

clean:
	@rm -rf $(BUILD_DIR) $(EXE)
