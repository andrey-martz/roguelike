#pragma once

#include <chrono>
#include <string>
#include <vector>

#include "utilities.h"

class Object {
    uint _pos_x{0}, _pos_y{0};  // the left up corner
    uint _width{1}, _height{1};

    std::vector<std::wstring> _image;

   public:
    Object() {}
    Object(const Object&);

    Object(const wchar_t*, const uint& = 0, const uint& = 0);

    Object(std::vector<std::wstring>&, const uint& = 0, const uint& = 0);

    const uint pos_x() const {
        return _pos_x;
    }
    const uint pos_y() const {
        return _pos_y;
    }
    const uint width() const {
        return _width;
    }
    const uint height() const {
        return _height;
    }
    const std::vector<std::wstring> image() const {
        return _image;
    }

    void move(const uint&, const uint&);

    virtual ~Object() {}
};

class Monster : public Object {
    std::chrono::time_point<std::chrono::steady_clock> _point{std::chrono::steady_clock::now()};

    Strategy _strategy{Strategy::Random};

    uint _max_hp{1}, _max_atk{1};
    uint _hp{1}, _atk{1};

    int _id;

   public:
    using Object::Object;

    Monster(const Monster& a) : Object(a) {}

    Monster operator=(const Monster& a) {
        _point = a._point;
        _strategy = a._strategy;
        _max_hp = a._max_hp;
        _max_atk = a._max_atk;
        _hp = a._hp;
        _atk = a._atk;

        _id = a._id;

        Object::operator=(a);
        return *this;
    }

    void set_id(int id) {
        _id = id;
    }
    const int id() const {
        return _id;
    }

    void set_point(int delay) {
        _point = std::chrono::steady_clock::now() + std::chrono::milliseconds(delay);
    }

    bool is_ready() const {
        return std::chrono::steady_clock::now() >= _point;
    }

    std::pair<uint, uint> generate_step(Strategy);

    std::pair<uint, uint> generate_step() {
        return generate_step(_strategy);
    }

    const uint max_health() const {
        return _max_hp;
    }
    const uint health() const {
        return _hp;
    }
    const uint max_attack() const {
        return _max_atk;
    }
    const uint attack() const {
        return _atk;
    }

    bool is_alive();
    void receive_damage(const uint&);

    ~Monster() {}
};