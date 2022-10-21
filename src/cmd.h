// vi: ft=c

#include "prompt.h"

#define MIN_CHAR 32
#define MAX_CHAR 126

static const chtype BULLET = '*';

typedef struct Command Command;

struct Password {
  int len;
  char value[MAX_PASSWORD_LEN];
};

struct Command {
  bool running;
  Prompt prompt;
  struct Password password;
};

Command new_command();

void run_command(Command *cmd);

void handle_key(Command *, int);
