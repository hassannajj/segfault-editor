#include <curses.h>

int main() {
    initscr();
    noecho();
    cbreak();

    mvaddch(5, 10, 'X');  // Draw X at position (5,10)
    getch();              // Nothing should be visible here
    refresh();            // Now it shows up
    getch();              // Press again to quit
    endwin();
    return 0;
}

