// vi: ft=c

#include <ncurses.h>

#define PROMPT_H 4
#define PROMPT_W 64

typedef struct Prompt prompt_t;

struct Prompt {
  int x;
  int y;
  WINDOW *win;
  WINDOW *input;
};

prompt_t new_prompt();

void paint_prompt(prompt_t *);
