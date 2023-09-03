/*
 * Copyright (c) 2022 Martin Helmut Fieber <info@martin-fieber.se>
 */

#pragma once

#include <memory>
#include <string>

#include "Core/Window.hpp"
#include "Raytracer/raytracer.hpp"

namespace App {

enum class ExitStatus : int { SUCCESS = 0, FAILURE = 1 };

class Application {
 public:
  explicit Application(const std::string& title);
  ~Application();

  Application(const Application&) = delete;
  Application(Application&&) = delete;
  Application& operator=(Application other) = delete;
  Application& operator=(Application&& other) = delete;

  ExitStatus run();
  void stop();

  void on_close();

 private:
  ExitStatus m_exit_status{ExitStatus::SUCCESS};
  std::unique_ptr<Window> m_window{nullptr};
  std::unique_ptr<Raytracer> m_raytracer{nullptr};
  std::vector<unsigned char> m_texture;

  bool m_running{true};
  bool m_show_some_panel{true};
};

}  // namespace App
