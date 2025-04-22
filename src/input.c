#include "input.h"
#include <ncurses.h>

InputResult read_input() {
  switch (ch) {
    case KEY_LEFT:
      result.type = INPUT_MOVE_LEFT;
      break;
    case KEY_RIGHT:
      result.type = INPUT_MOVE_RIGHT;
      break;
    case KEY_UP:
      result.type = INPUT_MOVE_UP;
      break;
    case KEY_DOWN:
      result.type = INPUT_MOVE_DOWN;
      break;
    case 127:  // Backspace
      result.type = INPUT_DELETE_CHAR;
      break;
    case 10:  // Enter
      result.type = INPUT_INSERT_CHAR;
      result.data = '\n';
      break;
    case 19:  // Ctrl+S
      result.type = INPUT_SAVE;
      break;
    case 17:  // Ctrl+Q
      result.type = INPUT_QUIT;
      break;
    default:
      if(isprint(ch)) {
        result.type = INPUT_INSERT_CHAR;
        result.data = (char)ch;
      } else {
        result.type = INPUT_UNKNOWN:
      }
  }
  return result;
}
