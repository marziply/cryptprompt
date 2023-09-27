#include <ncurses.h>

#define PROMPT_H 4
#define PROMPT_W 64
#define MIN_CHAR 32
#define MAX_CHAR 126
#define MAX_PASSWORD_LEN 80
#define PASSWORD_CHAR '*'
#define TITLE "Enter password"

typedef struct _win_st win_t;
typedef struct crypt_device crypt_dev_t;
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
  char value[MAX_PASSWORD_LEN + 1];
};

struct Prompt {
  int x;
  int y;
  int attempts;
  password_t password;
  win_t *win;
  win_t *input;
};

prompt_t new_prompt();

prompt_action_t handle_key(prompt_t *, int);

bool tick_prompt(prompt_t *);

void paint_prompt(prompt_t *);

void clear_password(prompt_t *);

void show_activity_sign(prompt_t *);

void incremement_attempts(prompt_t *);
