#pragma once

#include <string>
#include <vector>

#include "utilities.h"
class Map {
    enum { MAX_LENGTH = 2048 };
    std::vector<std::wstring> _map;

    std::wstring steppable{L" ."};

    size_t _width{0};
    size_t _height{0};

   public:
    Map(){};
    Map(std::string filename);
    Map(const std::vector<std::wstring>& m);

    const std::wstring operator[](int h) const;

    const size_t height() const {
        return _height;
    }
    const size_t width() const {
        return _width;
    }

    Map part(const uint& w, const uint& h, const uint& width, const uint& height) const;
    bool is_steppable(const uint& x, const uint& y) const;
};