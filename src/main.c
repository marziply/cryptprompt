#include "prompt.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libcryptsetup.h>

#define KEY_FILE "/tmp/key"

void win_init() {
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

int unlock(prompt_t *p, char *name, crypt_dev_t *cdev) {
  return crypt_activate_by_passphrase(
    cdev,
    name,
    CRYPT_ANY_SLOT,
    p->password.value,
    p->password.len,
    0
  );
}

bool attempt(prompt_t *p, char *name, crypt_dev_t *cdev) {
  show_activity_sign(p);

  if (unlock(p, name, cdev) >= 0) {
    return true;
  }

  inc_attempts(p);

  return false;
}

int main(int, char **argv) {
  if (getuid()) {
    printf("Execute as root to continue\n");

    return 1;
  }

  FILE *maybe_tty = fopen("/dev/tty", "w");
  FILE *maybe_tty0 = maybe_tty ? maybe_tty : fopen("/dev/tty0", "w");
  FILE *maybe_fb0 = maybe_tty0 ? maybe_tty0 : fopen("/dev/fb0", "w");
  FILE *out = maybe_fb0 ? maybe_fb0 : stdout;

  crypt_dev_t *cdev = NULL;
  char *dev = argv[1];
  char *name = argv[2];

  if (!dev) {
    printf("Missing device parameter\n");

    return 1;
  }

  if (!out) {
    printf("Target tty unreachable\n");

    return 1;
  }

  crypt_init(&cdev, dev);
  crypt_load(cdev, CRYPT_LUKS2, NULL);

  newterm(getenv("TERM"), out, stdin);
  win_init();

  prompt_t prompt = new_prompt();

  for (;;) {
    if (access("/.done", F_OK) == 0) {
      break;
    }

    if (tick_prompt(&prompt)) {
      continue;
    }

    if (attempt(&prompt, name, cdev)) {
      break;
    }
  }

  endwin();

  return 0;
}
