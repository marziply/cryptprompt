#include "cmd.h"
#include <string.h>

Command new_command() {
  Prompt prompt = new_prompt();

  return (Command) {
    true,
    prompt
  };
}

void run_command(Command *cmd) {
  // Main command event loop
  while (cmd->running) {
    paint_prompt(&cmd->prompt);
    handle_key(cmd, getch());

    refresh();
  }
}

// Removes the last key from the prompt and sets the last character in
// password to NULL
void del_key(WINDOW *input, struct Password *pw) {
  pw->value[pw->len - 1] = '\0';
  pw->len--;

  if (getcurx(input) == 0) {
    for (int i = 0; i < PROMPT_W - 3; i++) {
      mvwaddch(input, 0, i, BULLET);
    }
  } else {
    mvwdelch(input, getcury(input), getcurx(input) - 1);
  }
}

// Adds the pressed key character to the password
void add_key(WINDOW *input, struct Password *pw, char *key) {
  pw->len++;

  waddch(input, BULLET);
  strncat(pw->value, key, 1);
}

void handle_key(Command *c, int key) {
  switch (key) {
  // ESC
  case 27:
    c->running = false;
    break;
  case 10:
  case 13:
  case KEY_ENTER:
    c->running = false;
    break;
  case 8:
  case 127:
  case KEY_BACKSPACE:
    if (c->password.len) {
      del_key(c->prompt.input, &c->password);
    }
    break;
  default:
    if (c->password.len == MAX_PASSWORD_LEN) {
      // @TODO: Handle max password length here
      break;
    }

    if (key >= MIN_CHAR && key <= MAX_CHAR) {
      add_key(c->prompt.input, &c->password, (char *) &key);
    }
  }
}
