#include <curses.h>
#include <stdlib.h>
#include <signal.h>
#include "pt_core.h"
#include "input.h"

#define INITIAL_ADD_CAP 1024

typedef struct {
    int x, y;
} Cursor;

void move_left(Cursor *cursor) { 
  if (cursor->x > 0) {
    cursor->x--;
    move(cursor->y, cursor->x);
  }
}

void move_right(Cursor *cursor) {
  /*TODO: check condition of right boundary using length of line? */
    cursor->x++;
    move(cursor->y, cursor->x);
}

void move_up(Cursor *cursor) { 
  if (cursor->y > 0) {
    cursor->y++;
    move(cursor->y, cursor->x);
  }
}

void move_down(Cursor *cursor) {
  /*TODO: check condition of down boundary using number of lines? */
    cursor->y++;
    move(cursor->y, cursor->x);
}

void render_ncurses(PieceTable *pt) {
  mvprintw(13, 0, "pt char test");

  for (int i = 0; i < pt_content_len(pt); i++) {
    char c = pt_get_char_at(pt, i);
    mvprintw(14, i, "%c\n", c);
  }
}

static void finish(int sig) {
    endwin();
    /* do your non-curses wrapup here */
    exit(sig);
}

int main() {

  /* INITALIZATIONS */
  initscr(); /* initalizes curses library */
  signal(SIGINT, finish); /* clean exit on Ctrl-C */
  raw(); // Enables usage of ctrl+s and ctrl+q
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

  Cursor *cursor = malloc(sizeof(Cursor));
  PieceTable *pt = pt_init("", INITIAL_ADD_CAP);

  cursor->x = 0;
  cursor->y = 0;
  bool run = 1;
  while (run) {
    int c = getch();

    mvprintw(12, 0, "                                      ");
    // attrset(COLOR_PAIR(i % 7 + 1));
    
    InputResult input = read_input(c);
    
    switch (input.type) {
      case INPUT_QUIT:
        run = 0; 
        break;
      case INPUT_DELETE_CHAR:
        if (cursor->x > 0){
          cursor->x--;
          mvdelch(cursor->y, cursor->x);
        }
        break;
      case INPUT_ENTER_CHAR:
        cursor->y++;
        cursor->x = 0;
        break;

      case INPUT_MOVE_LEFT:
        move_left(cursor);
        break;
      case INPUT_MOVE_RIGHT:
        move_right(cursor);
        break;
      case INPUT_MOVE_UP:
        move_up(cursor);
        break;
      case INPUT_MOVE_DOWN:
        move_down(cursor);
        break;

      case INPUT_INSERT_CHAR:
        // Enter a character into ncurses buffer
        pt_insert_char(pt, c, cursor->x);
        mvaddch(cursor->y, cursor->x, c); // y set to 0 for now
        cursor->x++;
        break;
      default:
        // Unknown command
        mvprintw(12, 0, "Unknown command: key: %c, val: %d\n", c, c); /* DEBUG */
        
        break;
    }

    mvprintw(10, 0, "cursor x: %d, y: %d\nkey: %c, val: %d", cursor->y, cursor->x, c, c); /* DEBUG */
    move(cursor->y, cursor->x);
    render_ncurses(pt);
    refresh();
  }

  /* finish and clean up */
  free(cursor);
  finish(0);
}
