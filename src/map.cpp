#include <vector>
#include <fstream>
#include <iostream>
#include <string>

class Map {
    enum { MAX_LENGTH = 2048 };
    std::vector<std::wstring> _map;

    size_t _width{0};
    size_t _height{0};
public:
    Map(std::string filename) {
        FILE *f = fopen(filename.c_str(), "rt");
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
                _width = sizeof(str);
            }

            _map.emplace_back(s);
        }
        fclose(f);

        _height = _map.size();
    }

    Map(std::vector<std::wstring>& m) 
    : _map(m)
    , _width(0)
    , _height(m.size()) {
        for (auto & elem: m) {
            if (elem.size() > _width) {
                _width = elem.size();
            }
        }
    }

    const std::wstring operator[] (int h) const {
        if (h < 0 || h > _height) {
            return nullptr;
        }
        return _map[h];
    }

    const size_t height() const {
        return _height;
    }

    const size_t width() const {
        return _width;
    }

    Map part(const int w, const int h, const int width, const int height) const {
        std::vector<std::wstring> nw;
        
        for (int i = h; i < h + height && i < _height; ++i) {
            int n = (_map[i].size() - w) < width ? _map[i].size() - w : width;
            nw.push_back(_map[i].substr(w, n));
        }

        return Map(nw);
    }
};