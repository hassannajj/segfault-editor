#include <curses.h>
#include <stdlib.h>
#include <signal.h>
#include "pt_core.h"
#include "input.h"

static void finish(int sig) {
    endwin();
    /* do your non-curses wrapup here */
    exit(sig);
}

int main() {

  /* INITALIZATIONS */
  initscr(); /* initalizes curses library */
  signal(SIGINT, finish); /* clean exit on Ctrl-C */
  keypad(stdscr, TRUE);  /* enable keyboard mapping */

  //(void) nonl();         /* tell curses not to do NL->CR/NL on output */
  (void) cbreak();       /* take input chars one at a time, no wait for \n */
  (void) noecho();
  //(void) echo();         /* echo input - in color */


  /* color */ 
  if (has_colors()) {
    {
      start_color();
      /*
        * Simple color assignment, often all we need.  Color pair 0 cannot
        * be redefined.  This example uses the same value for the color
        * pair as for the foreground color, though of course that is not
        * necessary:
        */
      init_pair(1, COLOR_RED,     COLOR_BLACK);
      init_pair(2, COLOR_GREEN,   COLOR_BLACK);
      init_pair(3, COLOR_YELLOW,  COLOR_BLACK);
      init_pair(4, COLOR_BLUE,    COLOR_BLACK);
      init_pair(5, COLOR_CYAN,    COLOR_BLACK);
      init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
      init_pair(7, COLOR_WHITE,   COLOR_BLACK);
    }
  }

  unsigned int line = 0;
  unsigned int cursor = 0;
  bool run = 1;
  while (run) {
    int c = getch();
    // attrset(COLOR_PAIR(i % 7 + 1));
    
    InputResult input = read_input(c);
    
    switch (input.type) {
      case INPUT_QUIT:
        run = 0; 
        break;
      case INPUT_DELETE_CHAR:
        if (cursor > 0){
          cursor--;
          mvdelch(line, cursor);
        }
        break;
      case INPUT_ENTER_CHAR:
        line++;
        cursor = 0;
        break;

      case INPUT_MOVE_LEFT:
        move_cursor_left();
        break;
      case INPUT_MOVE_RIGHT:
        move_cursor_right();
        break;
      case INPUT_MOVE_UP:
        move_cursor_up();
        break;
      case INPUT_MOVE_DOWN:
        move_cursor_down();
        break;

      case INPUT_INSERT_CHAR:
        // Enter a character into ncurses buffer
        mvaddch(line, cursor, c); // y set to 0 for now
        cursor++;
        break;
      default:
        // Unknown command
        break;
    }

    mvprintw(10, 0, "line: %d, cursor: %d, key: %c, val: %d\n", line, cursor, c, c); /* DEBUG */
    move(line, cursor);
    refresh();
  }

  /* finish and clean up */
  finish(0);
}
