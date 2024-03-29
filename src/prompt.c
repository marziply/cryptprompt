#include "prompt.h"
#include <curses.h>
#include <string.h>
#include <stdlib.h>

prompt_t new_prompt() {
  password_t password = { 0 };
  prompt_t prompt = (prompt_t) {
    (COLS / 2) - (PROMPT_W / 2),
    (LINES / 2) - (PROMPT_H / 2),
    0,
    password,
    subwin(stdscr, PROMPT_H, PROMPT_W, prompt.y, prompt.x),
    derwin(prompt.win, 1, PROMPT_W - 2, 2, 1),
  };

  // Move to prompt window (center of screen)
  move(prompt.y + 2, prompt.x + 1);
  scrollok(prompt.input, true);

  return prompt;
}

// Start the event loop, paint on each iteration, and handle any incoming
// character key press
bool tick_prompt(prompt_t *p) {
  // Main command event loop
  paint_prompt(p);

  // Handle each character pressed
  switch (handle_key(p, getch())) {
  case PA_EXIT:
    return false;
  default:
    // @TODO: Handle other event types
    break;
  }

  refresh();

  return true;
}

// Red foreground colour if max password length is reached
int select_password_color(password_t *p) {
  int id = p->len == MAX_PASSWORD_LEN ? 2 : 1;

  return COLOR_PAIR(id);
}

// Paint the prompt window and prompt input
void paint_prompt(prompt_t *p) {
  size_t parens = sizeof(char) * 2;
  char *attempts = malloc(sizeof(p->attempts) + parens);
  int password_color = select_password_color(&p->password);

  wbkgd(p->win, COLOR_PAIR(1));
  wbkgd(p->input, password_color);
  wattron(p->input, A_BOLD);

  // Password prompt title
  mvwaddstr(p->win, 1, 1, "Enter password");

  if (p->attempts > 0) {
    sprintf(attempts, "(%d)", p->attempts);

    // wbkgd(p->win, COLOR_PAIR(2));
    mvwaddstr(p->win, 1, strlen(TITLE) + 2, attempts);
  }

  wbkgd(p->win, COLOR_PAIR(1));
  wmove(p->win, 2, p->password.len);

  wrefresh(p->win);
  wrefresh(p->input);
}

// Is the password empty (zero elements)?
bool is_password_empty(password_t *p) {
  return p->len == 0;
}

// Is the password length maxxed out?
bool is_password_full(password_t *p) {
  return p->len == MAX_PASSWORD_LEN;
}

// Empty password->value and delete the visible asterisks
void clear_password(prompt_t *p) {
  wdeleteln(p->input);
  wmove(p->input, 0, 0);

  wrefresh(p->win);
  wrefresh(p->input);

  p->password = (password_t) { 0 };
}

// Removes the last key from the prompt and sets the last character in
// password to NULL
void del_key(win_t *input, password_t *pw) {
  pw->value[--pw->len] = '\0';

  // Is the cursor at the start of the prompt?
  if (getcurx(input) == 0) {
    // Add PASSWORD_CHAR to the whole line
    for (int i = 0; i < PROMPT_W - 3; i++) {
      mvwaddch(input, 0, i, PASSWORD_CHAR);
    }
  } else {
    // Delete the last character from the end
    mvwdelch(input, getcury(input), getcurx(input) - 1);
  }
}

// Adds the pressed key character to the password
void add_key(win_t *input, password_t *pw, char *key) {
  pw->len++;

  waddch(input, PASSWORD_CHAR);
  strncat(pw->value, key, 1);
}

void show_activity_sign(prompt_t *p) {
  mvwaddstr(p->win, 1, PROMPT_W - 5, "[*]");
  wmove(p->win, 2, 1);
  wrefresh(p->win);
}

void inc_attempts(prompt_t *p) {
  p->attempts++;

  clear_password(p);

  wmove(p->win, 1, 0);
  wdeleteln(p->win);
}

prompt_action_t handle_key(prompt_t *p, int key) {
  switch (key) {
  case 0x0A:
  case 0x0D:
  case KEY_ENTER:
    return PA_EXIT;
  case 0x08:
  case 0x7F:
  case KEY_BACKSPACE:
    if (is_password_empty(&p->password)) {
      break;
    }

    del_key(p->input, &p->password);

    return PA_DELETE;
  // CTRL+U
  case 0x15:
    clear_password(p);
    break;
  default:
    if (is_password_full(&p->password)) {
      // @TODO: Handle max password length here
      break;
    }

    // Only handle key press if it is a valid alphanumeric
    if (key >= MIN_CHAR && key <= MAX_CHAR) {
      add_key(p->input, &p->password, (char *) &key);

      return PA_INSERT;
    }
  }

  return PA_NONE;
}
