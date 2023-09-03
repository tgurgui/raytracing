/*
 * Copyright (c) 2022 Martin Helmut Fieber <info@martin-fieber.se>
 */

#include "Window.hpp"

#include "Core/Debug/Instrumentor.hpp"
#include "Core/Log.hpp"

namespace App {

Window::Window(const Settings& settings) {
  APP_PROFILE_FUNCTION();

  m_window = std::make_shared<sf::RenderWindow>(sf::VideoMode(settings.width,
      settings.height), settings.title);
}

Window::~Window() {
  APP_PROFILE_FUNCTION();

}

std::shared_ptr<sf::RenderWindow> Window::get_native_window() const {
  return m_window;
}

}  // namespace App
