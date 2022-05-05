#include "environment.h"

#include <ncurses.h>

#include <algorithm>
#include <map>
#include <memory>
#include <vector>

void Environment::add_monster(const wchar_t* image, const uint& x, const uint& y) {
    auto res = std::find_if(_monsters.begin(), _monsters.end(),
                            [x, y](const Monster& m) {
                                return m.pos_x() == x && m.pos_y() == y;
                            });

    if (res == _monsters.end()) {
        _monsters.emplace_back(Monster(image, x, y));
    }
}

bool Environment::stop() {
    bool i = false;
    return shut.compare_exchange_strong(i, true);
}

void Environment::process() {
    int height, width;
    getmaxyx(_w, height, width);

    const int DIFF_SCROLL_H = height / 2 - 1;
    const int DIFF_SCROLL_W = width / 2 - 1;

    int w = 1, h = 1;
    int w_m = 0, h_m = 0;

    int64_t interval = double(1) / _fps * 1000;

    while (!shut) {
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

        auto part = _map.part(w_m, h_m, width, height);

        for (auto i = 0; i < part.height(); ++i) {
            mvwaddwstr(_w, i, 0, part[i].c_str());
        }

        std::for_each(_monsters.begin(), _monsters.end(),
                      [this, h_m, w_m](Monster& m) {
                          if (m.is_ready()) {
                              auto res = m.generate_step();

                              while (!_map.is_steppable(res.first, res.second)) {
                                  res = m.generate_step();
                              }

                              m.move(res.first, res.second);
                              m.set_point(2000);
                          }

                          init_pair(1, COLOR_BLUE, COLOR_NONE);
                          use_default_colors();

                          attron(COLOR_PAIR(1));
                          mvwaddwstr(_w, m.pos_y() - h_m, m.pos_x() - w_m, (m.image())[0].c_str());
                          attroff(COLOR_PAIR(1));
                      });

        // player
        init_pair(2, COLOR_MAGENTA, COLOR_NONE);
        use_default_colors();

        attron(COLOR_PAIR(2));
        const wchar_t* c = L"▲";
        mvwaddwstr(_w, h - h_m, w - w_m, c);
        attroff(COLOR_PAIR(2));

        timeout(interval);
        chtype in = getch();
        if (in != ERR) {
            in = toupper(in);

            std::pair<unsigned, unsigned> coord{w, h};

            if (in == _ctrl.up) {
                --coord.second;
            } else if (in == _ctrl.down) {
                ++coord.second;
            } else if (in == _ctrl.right) {
                ++coord.first;
            } else if (in == _ctrl.left) {
                --coord.first;
            } else {
                stop();
            }

            if (_map.is_steppable(coord.first, coord.second)) {
                auto res = std::find_if(_monsters.begin(), _monsters.end(), [coord](Monster m) {
                    return coord.first == m.pos_x() && coord.second == m.pos_y();
                });
                if (res == _monsters.end()) {
                    w = coord.first;
                    h = coord.second;
                }
            }
        }
    }
}