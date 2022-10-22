#include "cmd.h"
#include <string.h>

command_t new_command() {
  prompt_t prompt = new_prompt();

  return (command_t) {
    true,
    prompt
  };
}

// Begin the event loop and handle painting/key presses
void run_command(command_t *cmd) {
  // Main command event loop
  while (cmd->running) {
    paint_prompt(&cmd->prompt);
    handle_key(cmd, getch());

    refresh();
  }
}

// Empty password->value and delete the visible asterisks
void clear_password(WINDOW *input, password_t *pw) {
  wdeleteln(input);
  wmove(input, 0, 0);
  memset(pw->value, 0, sizeof(pw->value));
}

// Removes the last key from the prompt and sets the last character in
// password to NULL
void del_key(WINDOW *input, password_t *pw) {
  pw->value[pw->len - 1] = '\0';
  pw->len--;

  if (getcurx(input) == 0) {
    for (int i = 0; i < PROMPT_W - 3; i++) {
      mvwaddch(input, 0, i, PASSWORD_CHAR);
    }
  } else {
    mvwdelch(input, getcury(input), getcurx(input) - 1);
  }
}

// Adds the pressed key character to the password
void add_key(WINDOW *input, password_t *pw, char *key) {
  pw->len++;

  waddch(input, PASSWORD_CHAR);
  strncat(pw->value, key, 1);
}

void handle_key(command_t *c, int key) {
  switch (key) {
  case 0x0A:
  case 0x0D:
  case KEY_ENTER:
    c->running = false;
    break;
  case 0x08:
  case 0x7F:
  case KEY_BACKSPACE:
    if (c->password.len) {
      del_key(c->prompt.input, &c->password);
    }
    break;
  // CTRL+U
  case 0x15:
    clear_password(c->prompt.input, &c->password);
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
