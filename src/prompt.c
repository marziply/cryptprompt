#include "prompt.h"

Prompt new_prompt() {
  Prompt p = (Prompt) {
    (COLS / 2) - (PROMPT_W / 2),
    (LINES / 2) - (PROMPT_H / 2),
    subwin(stdscr, PROMPT_H, PROMPT_W, p.y, p.x),
    derwin(p.win, 1, PROMPT_W - 2, 2, 1),
  };

  // Move to prompt window (center of screen)
  move(p.y + 2, p.x + 1);
  scrollok(p.input, true);

  return p;
}

void paint_prompt(Prompt *p) {
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
