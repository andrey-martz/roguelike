#include "objects.h"

#include <atomic>
#include <chrono>
#include <string>
#include <thread>
#include <vector>

// object

Object::Object(const Object& other)
    : _pos_x(other._pos_x), _pos_y(other._pos_y), _width(other._width), _height(other._height), _image(other._image) {}

Object::Object(const wchar_t* image, const uint& x, const uint& y)
    : _image(std::vector<std::wstring>{image}), _pos_x(x), _pos_y(y) {
    _height = _image.size();

    _width = 0;
    for (auto& elem : _image) {
        if (elem.size() > _width) {
            _width = elem.size();
        }
    }
}
Object::Object(std::vector<std::wstring>& image, const uint& x, const uint& y)
    : _image(image), _pos_x(x), _pos_y(y) {
    _height = _image.size();

    _width = 0;
    for (auto& elem : _image) {
        if (elem.size() > _width) {
            _width = elem.size();
        }
    }
}
void Object::move(const uint& x, const uint& y) {
    if (_pos_x + x < 0) {
        _pos_x = 0;
    } else {
        _pos_x = x;
    }

    if (_pos_y + y < 0) {
        _pos_y = 0;
    } else {
        _pos_y = y;
    }
}

// monster

std::pair<uint, uint> Monster::generate_step(Strategy strategy) {
    if (strategy == Strategy::Random) {
        uint mv = std::rand() % 4;
        switch (mv) {
            case 0:
                return std::pair<uint, uint>(pos_x() + 1, pos_y());
            case 1:
                return std::pair<uint, uint>(pos_x() - 1, pos_y());
            case 2:
                return std::pair<uint, uint>(pos_x(), pos_y() + 1);
            case 3:
                return std::pair<uint, uint>(pos_x(), pos_y() - 1);
        }
    }
    return {};
}
