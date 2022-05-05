#pragma once

#include <ncurses.h>

#include <string>

struct Control {
   private:
    std::wstring avail = L"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

   public:
    wchar_t up{'W'}, down{'S'}, left{'A'}, right{'D'};
    wchar_t a_up{KEY_UP}, a_down{KEY_DOWN}, a_left{KEY_LEFT}, a_right{KEY_RIGHT};
    wchar_t spell{'R'};

    Control() {}
    Control(std::string filename);
};