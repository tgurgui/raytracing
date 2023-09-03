/*
 * Copyright (c) 2022 Martin Helmut Fieber <info@martin-fieber.se>
 */

#pragma once

#include <string>
#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>
namespace App {

class Window {
 public:
  struct Settings {
    std::string title;
    const int width{1280};
    const int height{720};
  };

  explicit Window(const Settings& settings);
  ~Window();

  Window(const Window&) = delete;
  Window(Window&&) = delete;
  Window& operator=(Window other) = delete;
  Window& operator=(Window&& other) = delete;

  [[nodiscard]] std::shared_ptr<sf::RenderWindow> get_native_window() const;

 private:
   std::shared_ptr<sf::RenderWindow> m_window{nullptr};
};

}  // namespace App
