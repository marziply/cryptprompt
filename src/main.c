#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PROMPT_H 4
#define PROMPT_W 64
#define MAX_PASSWORD_LEN 256
#define MIN_CHAR 32
#define MAX_CHAR 126

struct State {
  bool running;
  char password[MAX_PASSWORD_LEN];
};

struct Prompt {
  int y;
  int x;
  WINDOW *win;
  struct State *state;
};

void paint_prompt(struct Prompt *p) {
  wbkgd(p->win, COLOR_PAIR(1));
  wrefresh(p->win);
}

void handle_exit(struct State *s) {
  s->running = false;
}

void handle_enter(struct State *s) {
  handle_exit(s);
}

void handle_key(struct Prompt *p, int key) {
  int len = strlen(p->state->password);

  switch (key) {
  // ESC
  case 27:
    handle_exit(p->state);
    break;
  case 10:
  case 13:
  case KEY_ENTER:
    handle_enter(p->state);
    break;
  case 8:
  case 127:
  case KEY_BACKSPACE:
    if (len > 0) {
      p->state->password[len - 1] = '\0';

      wmove(p->win, getcury(p->win), getcurx(p->win) - 1);
      wdelch(p->win);
    }
    break;
  default:
    if (len < MAX_PASSWORD_LEN && key >= MIN_CHAR && key <= MAX_CHAR) {
      waddch(p->win, '*');
      strncat(p->state->password, (char *) &key, 1);
    }
  }
}

void init() {
  initscr();
  start_color();
  init_pair(1, COLOR_BLACK, COLOR_WHITE);
  noecho();
  cbreak();
  intrflush(stdscr, false);
  keypad(stdscr, true);
  box(stdscr, 0, 0);
}

int main(int, char **) {
  init();

  struct State state = { true };
  struct Prompt prompt = {
    (LINES / 2) - (PROMPT_H / 2),
    (COLS / 2) - (PROMPT_W / 2),
    subwin(stdscr, PROMPT_H, PROMPT_W, prompt.y, prompt.x),
    &state
  };

  // Password prompt title
  mvwaddstr(prompt.win, 1, 1, "Enter password");
  // Move to prompt window (center of screen)
  move(prompt.y + 2, prompt.x + 1);
  // Move cursor to input within prompt window
  wmove(prompt.win, 2, 1);

  while (state.running) {
    paint_prompt(&prompt);

    handle_key(&prompt, getch());

    refresh();
  }

  endwin();
  printf("%s\n", state.password);

  return 0;
}
