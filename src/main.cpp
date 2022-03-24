#include <iostream>
#include <ncurses.h>
#include <fstream>
#include <iomanip>
#include <vector>

#include "map.cpp"

int main() {
    setlocale(LC_CTYPE, "");

    std::ofstream log ("../log.txt", std::ios_base::app | std::ios_base::ate | std::ios_base::out);
    if (!log.is_open()) {
        std::cout << "unable to open log file." << std::endl;
        return 0;
    }
    setlocale(LC_CTYPE, "");
    initscr();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
	start_color();
    use_default_colors();

    Map mp("/home/andrey/roguelike/map");

    int row, col;
    getmaxyx(stdscr, row, col);

    int x = 1, y = 1;

    while (true) {
        clear();

        for (auto i = 0; i < mp.height(); ++i) {
            mvaddwstr(i, 0, mp[i].c_str());
        }

        const wchar_t *c = L"▲";
        mvaddwstr(y, x, c);

        refresh();
        chtype in = getch();
        in = toupper(in);
        bool flag = false;

        switch (in) {
            case KEY_UP:
            case 'W':
                if (mp[y - 1][x] == '.') {
                    --y;
                }
            break;
            case KEY_DOWN:
            case 'S':
                if (mp[y + 1][x] == '.') {
                    ++y;
                }
            break;
            case KEY_RIGHT:
            case 'D':
                if (mp[y][x + 1] == '.') {
                    ++x;
                }
            break;
            case KEY_LEFT:
            case 'A':
                if (mp[y][x - 1] == '.') {
                    --x;
                }
            break;
            default:
            flag = true;
        }
        if (flag) {
            auto t = std::time(nullptr);
            auto *tm = localtime(&t);

            log << "+ " << std::put_time(tm, "%Y/%m/%d %H:%M:%S ") << "get other key from WASD" << std::endl;

            break;
        }

    }

    clear();
    log.close();
    endwin();
}