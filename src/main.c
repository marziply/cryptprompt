#include "prompt.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

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

  prompt_t prompt = new_prompt();

  for (;;) {
    if (!tick_prompt(&prompt)) {
      break;
    }
  }

  endwin();

  printf("%s", prompt.password.value);

  return 0;
}
