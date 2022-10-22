#include "cmd.h"
#include <string.h>

command_t new_command() {
  prompt_t prompt = new_prompt();

  return (command_t) {
    /* true, */
    prompt
  };
}

// Begin the event loop and handle painting/key presses
void run_command(command_t *cmd) {
  // Main command event loop
  for (;;) {
    paint_prompt(&cmd->prompt);

    // Handle each character pressed
    switch (handle_key(&cmd->prompt, getch())) {
    case PA_EXIT:
      return;
    default:
      break;
    }

    refresh();
  }
}
