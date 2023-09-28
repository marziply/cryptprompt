CC = gcc
EXE = cryptprompt
FLAGS = -std=c2x -c -g -Wall
LIBS = -lncurses -lcryptsetup
BUILD_DIR = ./build
INSTALL_DIR = /usr/bin
INITCPIO_DIR = /etc/initcpio
SOURCES = $(wildcard src/*.c)
OBJECTS = $(patsubst src/%.c,build/%.o,$(SOURCES))

all: $(EXE)

$(EXE): $(notdir $(OBJECTS))
	$(CC) $(OBJECTS) $(LIBS) -o $@

%.o: src/%.c
	@mkdir -p $(BUILD_DIR)

	$(CC) $(FLAGS) $< -o $(BUILD_DIR)/$@

install: uninstall $(EXE)
	@cp ./$(EXE) $(INSTALL_DIR)/$(EXE)
	@cp ./bin/unlock $(INITCPIO_DIR)/hooks/unlock
	@cp ./bin/install $(INITCPIO_DIR)/install/unlock
	@mkinitcpio -P

uninstall:
	@rm -rf $(INSTALL_DIR)/$(EXE)
	@rm -rf $(INITCPIO_DIR)/{hooks,install}/unlock

clean:
	@rm -rf $(BUILD_DIR) $(EXE)
