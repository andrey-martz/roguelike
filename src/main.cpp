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

    int height, width;
    getmaxyx(stdscr, height, width);

    const int DIFF_SCROLL_H = height / 2 - 1;
    const int DIFF_SCROLL_W = width / 2 - 1;

    int w = 1, h = 1;
    int w_m = 0, h_m = 0;

    while (true) {
        clear();

        if (h_m > 0 && h < h_m + DIFF_SCROLL_H - 1) {
            h_m = h - (DIFF_SCROLL_H - 1) >= 0 ? h - (DIFF_SCROLL_H - 1) : 0;
        } else if (h >= h_m + height - DIFF_SCROLL_H) {
            h_m = h - height + DIFF_SCROLL_H;
        }

        if (w_m > 0 && w < w_m + DIFF_SCROLL_W - 1) {
            w_m = w - (DIFF_SCROLL_W - 1) >= 0 ? w - (DIFF_SCROLL_W - 1) : 0;
        } else if (w >= w_m + width - DIFF_SCROLL_W) {
            w_m = w - width + DIFF_SCROLL_W;
        }

        log << "coord: h = " << h << ", w = " << w << ", h_m = " << h_m << ", w_m = " << w_m << std::endl;

        auto part = mp.part(w_m, h_m, width, height);

        for (auto i = 0; i < part.height(); ++i) {
            mvaddwstr(i, 0, part[i].c_str());
        }

        const wchar_t *c = L"▲";
        mvaddwstr(h - h_m, w - w_m, c);

        refresh();
        chtype in = getch();
        in = toupper(in);
        bool flag = false;

        switch (in) {
            case KEY_UP:
            case 'W':
                if (mp[h - 1][w] == '.') {
                    --h;
                }
            break;
            case KEY_DOWN:
            case 'S':
                if (mp[h + 1][w] == '.') {
                    ++h;
                }
            break;
            case KEY_RIGHT:
            case 'D':
                if (mp[h][w + 1] == '.') {
                    ++w;
                }
            break;
            case KEY_LEFT:
            case 'A':
                if (mp[h][w - 1] == '.') {
                    --w;
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