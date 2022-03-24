#include <vector>
#include <fstream>
#include <iostream>
#include <string>

class Map {
    enum { MAX_LENGTH = 2048 };
    std::vector<std::wstring> _map;

    size_t _width;
    size_t _height;
public:
    Map(std::string filename)
    : _width(0)
    , _height(0) {
        std::wifstream file(filename, std::ios_base::in);
        if (!file.is_open()) {
            std::cout << "unable to open map file" << std::endl;
            return;
        }

        while (file.good() && !file.eof()) {
            wchar_t str[MAX_LENGTH];
            if (!file.getline(str, MAX_LENGTH, '\n')) {
                break;
            }

            std::wstring s(str);

            if (s.size() > _width) {
                _width = sizeof(str);
            }

            _map.emplace_back(s);
        }
        file.close();

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

    const std::wstring operator[] (int x) {
        return _map[x];
    }

    const size_t height() {
        return _height;
    }

    const size_t width() {
        return _width;
    }
};