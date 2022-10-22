#include <ncurses.h>

#define PROMPT_H 4
#define PROMPT_W 64
#define MIN_CHAR 32
#define MAX_CHAR 126
#define MAX_PASSWORD_LEN 80
#define PASSWORD_CHAR '*'

typedef struct Password password_t;
typedef struct Prompt prompt_t;
typedef enum PromptAction prompt_action_t;

enum PromptAction {
  PA_EXIT,
  PA_INSERT,
  PA_DELETE,
  PA_NONE
};

struct Password {
  int len;
  char value[MAX_PASSWORD_LEN];
};

struct Prompt {
  int x;
  int y;
  WINDOW *win;
  WINDOW *input;
  password_t password;
};

prompt_t new_prompt();

prompt_action_t handle_key(prompt_t *, int);

void paint_prompt(prompt_t *);
