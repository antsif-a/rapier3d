#include <iostream>

#include "rapier3d/window/framework/WindowFramework.hpp"

void WindowFramework::DefaultErrorCallback(int error_code, const char *description) {
    std::cerr << description << '\n';
}

