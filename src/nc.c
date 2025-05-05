#include <curses.h>
#include <stdlib.h>
#include <signal.h>
#include "pt_core.h"
#include "input.h"


#define INITIAL_ADD_CAP 1024

//TODO: add some sort of variable that will track where the cursor should be 
typedef struct {
    int x, y;
} Cursor;

void move_left(Cursor *cursor) { 
  if (cursor->x > 0) {
    cursor->x--;
    move(cursor->y, cursor->x);
  }
}

void move_right(PieceTable *pt, Cursor *cursor) {
  /* check condition of right boundary using length of current line  */
  if (cursor->x < pt_line_width(pt, cursor->y)-1) {
    cursor->x++;
    move(cursor->y, cursor->x);
  }
}

void move_up(PieceTable *pt, Cursor *cursor) { 
  if (cursor->y > 0) {
    cursor->y--;

    if (cursor->x > pt_line_width(pt, cursor->y)-1) {
      cursor->x = pt_line_width(pt, cursor->y) -1;
    }
    move(cursor->y, cursor->x);
  }
}

void move_down(PieceTable *pt, Cursor *cursor) {
  /* check condition of down boundary using number of lines */
  if (cursor->y < pt->num_lines-1) {
    cursor->y++;

    if (cursor->x > pt_line_width(pt, cursor->y)-1) {
      cursor->x = pt_line_width(pt, cursor->y) -1;
    }

    move(cursor->y, cursor->x);
  }
}

void render_ncurses(PieceTable *pt) {
  clear();
  int y = 0;
  int x = 0;

  for (int i = 0; i < pt->content_len; i++) {
    char c = pt_get_char_at_i(pt, i);
    if (c == '\n') {
      y++;
      x = 0;
      move(y, x);  // <--- move cursor manually when \n

    } else {
      mvaddch(y, x, c); // Prints one char at a time
      x ++;
    }
  }
  mvprintw(15, 0, "pt->content: %s\n", pt_get_content(pt));
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
  // When opening an empty file, always initialize with a single "\n" char
  PieceTable *pt = pt_init("hello\nworld\n", INITIAL_ADD_CAP);

  cursor->x = 0;
  cursor->y = 0;
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
        if (cursor->x > 0){
          cursor->x--;
          mvdelch(cursor->y, cursor->x);
        } else if (cursor->y > 0) {
          cursor->y--;
        }
        break;
      case INPUT_ENTER_CHAR:
        pt_insert_char_at_YX(pt, '\n', cursor->y, cursor->x);
        cursor->y++;
        cursor->x = 0;
        break;

      case INPUT_MOVE_LEFT:
        move_left(cursor);
        break;
      case INPUT_MOVE_RIGHT:
        move_right(pt, cursor);
        break;
      case INPUT_MOVE_UP:
        move_up(pt, cursor);
        break;
      case INPUT_MOVE_DOWN:
        move_down(pt, cursor);
        break;

      case INPUT_INSERT_CHAR:
        // Enter a character into ncurses buffer
        pt_insert_char_at_YX(pt, c, cursor->y, cursor->x);
        //mvaddch(cursor->y, cursor->x, c); // y set to 0 for now
        cursor->x++;
        break;
      default:
        // Unknown command
        mvprintw(9, 0, "Unknown command: key: %c, val: %d\n", c, c); /* DEBUG */
        
        break;
    }


    /* Renders the text in piece table */
    render_ncurses(pt);
    mvprintw(10, 0, "cursor y: %d, x: %d\nline_width: %d\nnum_lines: %d\nkey: %c, val: %d, \npt->content_len: %d", cursor->y, cursor->x, pt_line_width(pt, cursor->y), pt->num_lines, c, c, pt->content_len); /* DEBUG */

    /* Moves the cursor in the correct location */
    move(cursor->y, cursor->x); 

    refresh();

  }

  /* finish and clean up */
  free(cursor);
  finish(0);
}
