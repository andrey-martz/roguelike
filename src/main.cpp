#include <ncurses.h>
#include <unistd.h>

#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "control.cpp"
#include "map.cpp"
#include "objects.cpp"

int main() {
    enum {
        COLOR_NONE = -1
    };

    setlocale(LC_CTYPE, "");
    std::srand(std::time(nullptr));

    std::ofstream log("../log.txt", std::ios_base::app | std::ios_base::ate | std::ios_base::out);
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
    Control ctrl("/home/andrey/roguelike/control");

    int height, width;
    getmaxyx(stdscr, height, width);

    const int DIFF_SCROLL_H = height / 2 - 1;
    const int DIFF_SCROLL_W = width / 2 - 1;

    int w = 1, h = 1;
    int w_m = 0, h_m = 0;

    Monster monster(L"M", 15, 4);

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

        // log << "coord: h = " << h << ", w = " << w << ", h_m = " << h_m << ", w_m = " << w_m << std::endl;

        auto part = mp.part(w_m, h_m, width, height);

        for (auto i = 0; i < part.height(); ++i) {
            mvaddwstr(i, 0, part[i].c_str());
        }

        init_pair(1, COLOR_BLUE, COLOR_NONE);
        use_default_colors();

        attron(COLOR_PAIR(1));
        if (monster.height() == 1) {
            mvaddwstr(monster.pos_y() - h_m, monster.pos_x() - w_m, (monster.image())[0].c_str());
        }
        attroff(COLOR_PAIR(1));

        init_pair(2, COLOR_MAGENTA, COLOR_NONE);
        use_default_colors();

        attron(COLOR_PAIR(2));
        const wchar_t *c = L"▲";
        mvaddwstr(h - h_m, w - w_m, c);
        attroff(COLOR_PAIR(2));

        refresh();
        chtype in = getch();
        in = toupper(in);
        bool flag = false;

        if (in == ctrl.up) {
            if (mp.is_steppable(w, h - 1)) {
                --h;
            }
        } else if (in == ctrl.down) {
            if (mp.is_steppable(w, h + 1)) {
                ++h;
            }
        } else if (in == ctrl.right) {
            if (mp.is_steppable(w + 1, h)) {
                ++w;
            }
        } else if (in == ctrl.left) {
            if (mp.is_steppable(w - 1, h)) {
                --w;
            }
        } else {
            auto t = std::time(nullptr);
            auto *tm = localtime(&t);

            log << "+ " << std::put_time(tm, "%Y/%m/%d %H:%M:%S ") << "get other key from control: " << in << std::endl;

            break;
        }

        bool moved = false;
        while (!moved) {
            unsigned mv = std::rand() % 4;
            switch (mv) {
                case 0:
                    if (mp.is_steppable(monster.pos_x() + 1, monster.pos_y())) {
                        monster.move(1, 0);
                        moved = true;
                    }
                    break;
                case 1:
                    if (mp.is_steppable(monster.pos_x(), monster.pos_y() + 1)) {
                        monster.move(0, 1);
                        moved = true;
                    }
                    break;
                case 2:
                    if (mp.is_steppable(monster.pos_x() - 1, monster.pos_y())) {
                        monster.move(-1, 0);
                        moved = true;
                    }
                    break;
                case 3:
                    if (mp.is_steppable(monster.pos_x(), monster.pos_y() - 1)) {
                        monster.move(0, -1);
                        moved = true;
                    }
                    break;
            }
        }

        sleep(1 / 2);
    }

    clear();
    log.close();
    endwin();
}