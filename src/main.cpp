#include <iostream>
#include <ncurses.h>
#include <fstream>
#include <iomanip>

int main() {
    setlocale(LC_CTYPE, "");

    std::ofstream log ("../log.txt", std::ios_base::app | std::ios_base::ate | std::ios_base::out);
    if (!log.is_open()) {
        std::cout << "unable to open log file." << std::endl;
        return 0;
    }
    initscr();
    noecho();
    start_color();
    curs_set(0);
    keypad(stdscr, true);

    int row, col;
    getmaxyx(stdscr, row, col);

    int x = 1, y = 1;

    while (true) {
        clear();

        chtype c = '@' | A_BLINK;
        mvaddch(y, x, c);

        refresh();
        chtype in = getch();
        in = toupper(in);

        if (in == KEY_UP || in == 'W') {
            --y;
        } else if (in == KEY_DOWN || in == 'S') {
            ++y;
        } else if (in == KEY_RIGHT || in == 'D') {
            ++x;
        } else if (in == KEY_LEFT || in == 'A') {
            --x;
        } else {
            auto t = std::time(nullptr);
            auto *tm = localtime(&t);

            log << "+ " << std::put_time(tm, "%Y/%m/%d %H:%M:%S ") << "get other key from WASD" << std::endl;

            break;
        }
    }

    log.close();
    endwin();
}