// vi: ft=c

#include <ncurses.h>

#define PROMPT_H 4
#define PROMPT_W 64

typedef struct Prompt Prompt;

struct Prompt {
  int x;
  int y;
  WINDOW *win;
  WINDOW *input;
};

Prompt new_prompt();

void paint_prompt(Prompt *);
