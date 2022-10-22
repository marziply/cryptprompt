#include "prompt.h"

typedef struct Command command_t;

struct Command {
  // Might not be needed any more
  /* bool running; */
  prompt_t prompt;
};

command_t new_command();

password_t *get_password(command_t *cmd);

void run_command(command_t *cmd);
