#include "environment.h"

#include <ncurses.h>

#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <vector>

void Environment::add_monster(const wchar_t* image, const uint& x, const uint& y, const int& id) {
    Monster m = Monster(image, x, y);
    m.set_id(id);
    _monsters.emplace_back(m);
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

    std::chrono::time_point<std::chrono::steady_clock> m_point{std::chrono::steady_clock::now() + std::chrono::seconds(1)};
    int id = 0;

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

        // monster generator

        if (std::chrono::steady_clock::now() >= m_point) {
            m_point = std::chrono::steady_clock::now() + std::chrono::seconds(1);

            uint m_x = std::rand() % _map.width();
            uint m_y = std::rand() % _map.height();

            while (!_map.is_steppable(m_x, m_y) || (m_x == w && m_y == h)) {
                m_x = std::rand() % _map.width();
                m_y = std::rand() % _map.height();
            }

            add_monster(L"M", m_x, m_y, id);
            ++id;
        }

        //

        for (auto& m : _monsters) {
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
        }

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
                auto it = _monsters.begin();
                for (; it != _monsters.end(); ++it) {
                    if (coord.first == it->pos_x() && coord.second == it->pos_y()) {
                        break;
                    }
                }

                if (it == _monsters.end()) {
                    w = coord.first;
                    h = coord.second;
                } else {
                    it->receive_damage(1);
                    if (!it->is_alive()) {
                        _monsters.erase(it);
                    }
                }
            }
        }
    }
}