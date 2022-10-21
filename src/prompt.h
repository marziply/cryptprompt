// vi: ft=c

#include <ncurses.h>

#define PROMPT_H 4
#define PROMPT_W 64
#define MAX_PASSWORD_LEN 80

typedef struct Prompt Prompt;

struct Prompt {
  int x;
  int y;
  WINDOW *win;
  WINDOW *input;
};

Prompt new_prompt();

void paint_prompt(Prompt *);
