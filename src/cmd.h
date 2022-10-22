// vi: ft=c

#include "prompt.h"

#define MIN_CHAR 32
#define MAX_CHAR 126
#define MAX_PASSWORD_LEN 80

static const char PASSWORD_CHAR = '*';

typedef struct Command command_t;
typedef struct Password password_t;

struct Password {
  int len;
  char value[MAX_PASSWORD_LEN];
};

struct Command {
  bool running;
  prompt_t prompt;
  password_t password;
};

command_t new_command();

void run_command(command_t *cmd);

void handle_key(command_t *, int);
