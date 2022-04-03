#include <atomic>
#include <chrono>
#include <string>
#include <thread>
#include <vector>

class Object {
    unsigned _pos_x{0}, _pos_y{0};  // the left up corner
    unsigned _width{1}, _height{1};

    std::vector<std::wstring> _image;

   public:
    Object() {}

    Object(const wchar_t *image, unsigned x = 0, unsigned y = 0)
        : _image(std::vector<std::wstring>{image}), _pos_x(x), _pos_y(y) {
        _height = _image.size();

        _width = 0;
        for (auto &elem : _image) {
            if (elem.size() > _width) {
                _width = elem.size();
            }
        }
    }

    Object(std::vector<std::wstring> &image, unsigned x = 0, unsigned y = 0)
        : _image(image), _pos_x(x), _pos_y(y) {
        _height = _image.size();

        _width = 0;
        for (auto &elem : _image) {
            if (elem.size() > _width) {
                _width = elem.size();
            }
        }
    }

    const unsigned pos_x() const {
        return _pos_x;
    }
    const unsigned pos_y() const {
        return _pos_y;
    }
    const unsigned width() const {
        return _width;
    }
    const unsigned height() const {
        return _height;
    }
    void move(unsigned x, unsigned y) {
        if (_pos_x + x < 0) {
            _pos_x = 0;
        } else {
            _pos_x += x;
        }

        if (_pos_y + y < 0) {
            _pos_y = 0;
        } else {
            _pos_y += y;
        }
    }
    const std::vector<std::wstring> image() {
        return _image;
    }
};

class Monster : public Object {
    std::atomic_bool _ready_to_move{false};

   public:
    using Object::Object;

    void set_timer(int delay) {
        std::thread([=]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            _ready_to_move.store(true);
        }).detach();
    }

    bool is_ready() {
        bool exp = true;
        return _ready_to_move.compare_exchange_strong(exp, false);
    }
};

class Player : Object {
    using Object::Object;
};