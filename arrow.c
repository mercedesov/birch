#include <ncurses.h>

int main() {
    int key, x = 0, y = 1;

    initscr();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    mvprintw(0, 0, "Use arrows to draw. c - clear, q - quit");
    move(y, x);
    printw("0");
    refresh();

    do {
        key = getch();

        if (key == 'c') {
            clear();
            mvprintw(0, 0, "Use arrows to draw. c - clear, q - quit");
            x = 0;
            y = 1;
        } else if (key == KEY_LEFT && x > 0) {
            x--;
        } else if (key == KEY_RIGHT && x < max_x - 1) {
            x++;
        } else if (key == KEY_UP && y > 1) {
            y--;
        } else if (key == KEY_DOWN && y < max_y - 1) {
            y++;
        }

        mvprintw(y, x, "0");
        refresh();
    } while (key != 'q');

    endwin();
    return 0;
}

