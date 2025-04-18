#include <curses.h>
#include <stdlib.h>
#include <signal.h>


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

  int i = 0;
  while (1) {
    int c = getch();
    // attrset(COLOR_PAIR(i % 7 + 1));

    if (c == '\n') break;
    if (c == KEY_BACKSPACE || c == KEY_DC || c == 127) {
      if (i > 0) {
        i --;
        mvdelch(0, i); // y set to 0 for now
      }
    }

    else {
      mvaddch(0, i, c); // y set to 0 for now
      i++;

    }
    mvprintw(2, 0, "i: %d, key: %c, val: %d\n", i, c, c); /* DEBUG */
    move(0, i);

    refresh();
  }

  /* finish and clean up */
  finish(0);
}
