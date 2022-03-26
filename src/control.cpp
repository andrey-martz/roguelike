#include <fstream>
#include <ncurses.h>

struct Control {
private:
    std::wstring avail = L"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
public:
    wchar_t up, down, left, right;
    wchar_t a_up, a_down, a_left, a_right;
    wchar_t spell;

    Control(std::string filename) {
        std::wifstream file(filename);
        std::wstring str;

        while (file >> str) {
            int pos = str.find(L"=");
            if (pos == std::string::npos) {
                return;
            }

            std::wstring s_action = str.substr(0, pos);
            std::wstring s_key = str.substr(pos + 1, str.size() - pos - 1);

            wchar_t key;
            if (s_key.size() == 1) {
                if (avail.find(s_key[0]) != std::string::npos) {
                    key = toupper(s_key[0]);
                }
            } else {
                if (s_key.compare(L"KEY_UP") == 0) {
                    key = KEY_UP;
                } else if (s_key.compare(L"KEY_DOWN") == 0) {
                    key = KEY_DOWN;
                } else if (s_key.compare(L"KEY_LEFT") == 0) {
                    key = KEY_LEFT;
                } else if (s_key.compare(L"KEY_RIGHT") == 0) {
                    key = KEY_RIGHT;
                } else {
                    return;
                }
            }

            if (s_action == L"UP") {
                up = key;
            } else if (s_action == L"DOWN") {
                down = key;
            } else if (s_action == L"LEFT") {
                left = key;
            } else if (s_action == L"RIGHT") {
                right = key;
            } else if (s_action == L"ATTACK_UP") {
                a_up = key;
            } else if (s_action == L"ATTACK_DOWN") {
                a_down = key;
            } else if (s_action == L"ATTACK_LEFT") {
                a_left = key;
            } else if (s_action == L"ATTACK_RIGHT") {
                a_right = key;
            } else if (s_action == L"SPELL") {
                spell = key;
            }
        }
    }
};