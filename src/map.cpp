#include "map.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

Map::Map(std::string filename) {
    FILE* f = fopen(filename.c_str(), "rt");
    if (f == nullptr) {
        return;
    }

    while (!feof(f)) {
        wchar_t str[MAX_LENGTH];

        if (!fgetws(str, MAX_LENGTH, f)) {
            break;
        }

        std::wstring s(str);

        if (s.size() > _width) {
            _width = s.size();
        }

        _map.emplace_back(s);
    }
    fclose(f);

    _height = _map.size();
}

Map::Map(const std::vector<std::wstring>& m)
    : _map(m), _width(0), _height(m.size()) {
    for (auto& elem : m) {
        if (elem.size() > _width) {
            _width = elem.size();
        }
    }
}

const std::wstring Map::operator[](int h) const {
    if (h < 0 || h > _height) {
        return std::wstring(L"");
    }
    return _map[h];
}

Map Map::part(const uint& w, const uint& h, const uint& width, const uint& height) const {
    std::vector<std::wstring> nw;

    for (int i = h; i < h + height && i < _height; ++i) {
        int n = (int(_map[i].size()) - w) < width ? _map[i].size() - w : width;
        if (n < 0 || _map[i].length() < w) {
            nw.push_back(std::wstring(L""));
        } else {
            nw.push_back(_map[i].substr(w, n));
        }
    }

    return Map(nw);
}

bool Map::is_steppable(const uint& x, const uint& y) const {
    if (y >= _height) {
        return false;
    }
    if (x >= _map[y].size()) {
        return false;
    }

    return steppable.find(_map[y][x]) != std::string::npos;
}