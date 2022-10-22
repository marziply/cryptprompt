#include "prompt.h"
#include <string.h>

prompt_t new_prompt() {
  password_t password = { 0 };
  prompt_t prompt = (prompt_t) {
    (COLS / 2) - (PROMPT_W / 2),
    (LINES / 2) - (PROMPT_H / 2),
    subwin(stdscr, PROMPT_H, PROMPT_W, prompt.y, prompt.x),
    derwin(prompt.win, 1, PROMPT_W - 2, 2, 1),
    password
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
    break;
  }

  refresh();

  return true;
}

// Paint the prompt window and prompt input
void paint_prompt(prompt_t *p) {
  // Red foreground colour if max password length is reached
  /* int color_id = p->state->len == MAX_PASSWORD_LEN ? 2 : 1; */

  wbkgd(p->win, COLOR_PAIR(1));
  /* wbkgd(p->input, COLOR_PAIR(color_id)); */
  wbkgd(p->input, COLOR_PAIR(1));
  wattron(p->input, A_BOLD);

  // Password prompt title
  mvwaddstr(p->win, 1, 1, "Enter password");

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
void clear_password(WINDOW *input, password_t *pw) {
  wdeleteln(input);
  wmove(input, 0, 0);
  memset(pw->value, 0, sizeof(pw->value));
}

// Removes the last key from the prompt and sets the last character in
// password to NULL
void del_key(WINDOW *input, password_t *pw) {
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
void add_key(WINDOW *input, password_t *pw, char *key) {
  pw->len++;

  waddch(input, PASSWORD_CHAR);
  strncat(pw->value, key, 1);
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
    clear_password(p->input, &p->password);
    break;
  default:
    if (is_password_full(&p->password)) {
      // @TODO: Handle max password length here
      break;
    }

    if (key >= MIN_CHAR && key <= MAX_CHAR) {
      add_key(p->input, &p->password, (char *) &key);

      return PA_INSERT;
    }
  }

  return PA_NONE;
}
