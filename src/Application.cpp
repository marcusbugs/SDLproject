#include "Application.hpp"
#include <iostream>

Application::~Application() {
    delete renderer;
    SDL_Quit();
}

bool Application::Init(int windowWidth, int windowHeight) {

    return false;
}

void Application::Input() {

}

void Application::Update() {

}

void Application::Render() {

}