#include "prompt.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define KEY_FILE "/tmp/key"
#define SILENT "> /dev/null 2>&1"

void init() {
  // Colours
  start_color();
  init_pair(1, COLOR_BLACK, COLOR_WHITE);
  init_pair(2, COLOR_RED, COLOR_WHITE);

  // Options
  noecho();
  cbreak();
  curs_set(0);
  intrflush(stdscr, false);
  keypad(stdscr, true);

  // Appearance
  box(stdscr, 0, 0);
}

int main(int argc, char **argv) {
  if (getuid()) {
    printf("Execute as root to continue\n");

    return 1;
  }

  FILE *tty = fopen("/dev/tty", "w");
  FILE *target = tty ? tty : stdout;

  char *base_cmd = "/sbin/cryptsetup open -d /tmp/key --test-passphrase";
  char *setup_cmd = malloc(strlen(base_cmd) + strlen(SILENT) + sizeof(argv));

  newterm(getenv("TERM"), target, stdin);

  init();

  prompt_t prompt = new_prompt();

  strcat(setup_cmd, base_cmd);

  for (int i = 1; i < argc; i++) {
    strcat(setup_cmd, " ");
    strcat(setup_cmd, argv[i]);
  }

  strcat(setup_cmd, SILENT);

  for (;;) {
    if (!tick_prompt(&prompt)) {
      FILE *key = fopen(KEY_FILE, "w");

      fprintf(key, "%s", prompt.password.value);
      fclose(key);

      clear_password(&prompt);

      mvwaddstr(prompt.win, 1, PROMPT_W - 5, "[*]");
      wmove(prompt.win, 2, 1);
      wrefresh(prompt.win);

      if (system(setup_cmd) == 0) {
        remove(KEY_FILE);

        break;
      }

      prompt.attempts++;

      wmove(prompt.win, 1, 0);
      wdeleteln(prompt.win);

      remove(KEY_FILE);
    }
  }

  endwin();

  // printf("%s", prompt.password.value);

  return 0;
}
