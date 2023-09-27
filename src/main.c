#include "prompt.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libcryptsetup.h>

#define KEY_FILE "/tmp/key"

void window_init() {
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

int unlock(prompt_t *p, char *name, crypt_dev_t *cd) {
  return crypt_activate_by_passphrase(
    cd,
    name,
    CRYPT_ANY_SLOT,
    p->password.value,
    p->password.len,
    0
  );
}

int main(int, char **argv) {
  if (getuid()) {
    printf("Execute as root to continue\n");

    return 1;
  }

  FILE *tty = fopen("/dev/tty", "w");
  FILE *target = tty ? tty : stdout;

  crypt_dev_t *cd = NULL;
  char *dev = argv[1];
  char *name = argv[2];

  if (!dev) {
    printf("Missing device parameter\n");

    return 1;
  }

  crypt_init(&cd, dev);
  crypt_load(cd, CRYPT_LUKS2, NULL);

  newterm(getenv("TERM"), target, stdin);
  window_init();

  prompt_t prompt = new_prompt();

  for (;;) {
    if (tick_prompt(&prompt)) {
      continue;
    }

    show_activity_sign(&prompt);

    if (unlock(&prompt, name, cd) >= 0) {
      break;
    }

    incremement_attempts(&prompt);
  }

  endwin();

  return 0;
}
