#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PROMPT_H 4
#define PROMPT_W 64
/* #define MAX_PASSWORD_LEN 128 */
#define MAX_PASSWORD_LEN 80
#define MIN_CHAR 32
#define MAX_CHAR 126

static const chtype bullet = '*';

struct State {
  bool running;
  int len;
  char password[MAX_PASSWORD_LEN];
};

struct Prompt {
  int y;
  int x;
  WINDOW *win;
  WINDOW *input;
  struct State *state;
};

void paint_prompt(struct Prompt *p) {
  // Red foreground colour if max password length is reached
  int color_id = p->state->len == MAX_PASSWORD_LEN ? 2 : 1;

  wbkgd(p->win, COLOR_PAIR(1));
  wbkgd(p->input, COLOR_PAIR(color_id));
  wattron(p->input, A_BOLD);

  // Password prompt title
  mvwaddstr(p->win, 1, 1, "Enter password");

  wrefresh(p->win);
  wrefresh(p->input);
}

void handle_exit(struct State *s) {
  s->running = false;
}

void handle_enter(struct State *s) {
  handle_exit(s);
}

void handle_key(struct Prompt *p, int key) {
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
    if (p->state->len > 0) {
      p->state->password[p->state->len - 1] = '\0';
      p->state->len--;

      if (getcurx(p->input) == 0) {
        for (int i = 0; i < PROMPT_W - 3; i++) {
          mvwaddch(p->input, 0, i, bullet);
        }
      } else {
        mvwdelch(p->input, getcury(p->input), getcurx(p->input) - 1);
      }
    }
    break;
  default:
    if (p->state->len == MAX_PASSWORD_LEN) {
      // @TODO: Handle max password length here
      break;
    }

    if (key >= MIN_CHAR && key <= MAX_CHAR) {
      p->state->len++;

      waddch(p->input, bullet);
      strncat(p->state->password, (char *) &key, 1);
    }
  }
}

void init() {
  initscr();

  // Colours
  start_color();
  init_pair(1, COLOR_BLACK, COLOR_WHITE);
  init_pair(2, COLOR_RED, COLOR_WHITE);

  // Options
  noecho();
  cbreak();
  intrflush(stdscr, false);
  keypad(stdscr, true);

  // Appearance
  box(stdscr, 0, 0);
}

int main(int, char **) {
  init();

  struct State state = { true };
  struct Prompt prompt = {
    (LINES / 2) - (PROMPT_H / 2),
    (COLS / 2) - (PROMPT_W / 2),
    subwin(stdscr, PROMPT_H, PROMPT_W, prompt.y, prompt.x),
    derwin(prompt.win, 1, PROMPT_W - 2, 2, 1),
    &state
  };

  // Move to prompt window (center of screen)
  move(prompt.y + 2, prompt.x + 1);
  scrollok(prompt.input, true);

  while (state.running) {
    paint_prompt(&prompt);

    handle_key(&prompt, getch());

    refresh();
  }

  endwin();
  printf("%s\n", state.password);

  return 0;
}
