#include "Application.hpp"

int main(int argc, char* argv[]) {
    Application app;

    if (!app.Init(800, 800)) return 1;

    while (app.IsRunning()) {
        app.Input();
        app.Update();
        app.Render();
    }

    return 0;
}