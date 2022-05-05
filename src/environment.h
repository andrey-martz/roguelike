#pragma once

#include <atomic>
#include <vector>

#include "control.h"
#include "map.h"
#include "objects.h"

class Environment {
    Map _map;
    Control _ctrl;

    std::vector<Monster> _monsters;
    WINDOW* _w;
    std::atomic<bool> shut{false};

    unsigned _fps{10};

   public:
    Environment(unsigned fps = 10, WINDOW* w = stdscr)
        : _fps{fps}, _w(w) {
    }
    Environment(const Map& map, unsigned fps = 10, WINDOW* w = stdscr)
        : _map(map), _fps(fps), _w(w) {
    }

    void start() {
        process();
    }

    const Map& map() {  // can be changed
        return _map;
    }

    void add_monster(const wchar_t* image, const uint& x = 0, const uint& y = 0);
    bool stop();
    void process();

    ~Environment() {}
};