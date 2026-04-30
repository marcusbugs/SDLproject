#include <iostream>
#include <ostream>

#include "Color.hpp"
#include "Application.hpp"
#include "Vec2.hpp"

int main(int argc, char* argv[]) {
    Vec2 a(10, 20);
    Vec2 b(20, -20);
    std::cout << a.length() << std::endl;
    std::cout << b.length() << std::endl;
    Vec2 c = a + b;
    std::cout << c.length() << std::endl;

    Application app;

    if (!app.Init(800, 800)) return 1;

    while (app.IsRunning()) {
        app.Input();
        app.Update();
        app.Render();
    }

    return 0;
}