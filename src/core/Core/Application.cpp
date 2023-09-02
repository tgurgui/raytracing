/*
 * Copyright (c) 2022 Martin Helmut Fieber <info@martin-fieber.se>
 */

#include "Application.hpp"

#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

#include "Core/Debug/Instrumentor.hpp"

namespace App {

Application::Application(const std::string& title) {
  APP_PROFILE_FUNCTION();

  m_window = std::make_unique<Window>(Window::Settings{title});
  m_raytracer = std::make_unique<Raytracer>(Raytracer{});
}

Application::~Application() {
  APP_PROFILE_FUNCTION();

  ImGui::SFML::Shutdown();
}

ExitStatus App::Application::run() {
  APP_PROFILE_FUNCTION();

  if (m_exit_status == ExitStatus::FAILURE) {
    return m_exit_status;
  }

  // Setup Dear ImGui context
  //IMGUI_CHECKVERSION();
  //ImGui::CreateContext();
  //ImGuiIO& io{ImGui::GetIO()};

  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable |
  //                  ImGuiConfigFlags_ViewportsEnable;

  // ImGUI font
  /*const float font_scaling_factor{m_window->get_scale()};
  const float font_size{18.0F * font_scaling_factor};
  io.Fonts->AddFontFromFileTTF("fonts/Manrope.ttf", font_size);
  io.FontDefault = io.Fonts->AddFontFromFileTTF("fonts/Manrope.ttf", font_size);
  io.FontGlobalScale = 1.0F / font_scaling_factor;*/

  // Setup Platform/Renderer backends
  ImGui::SFML::Init(*m_window->get_native_window());
  sf::Clock deltaClock;
  m_texture.resize(m_raytracer->width() * m_raytracer->height() * 4);
  sf::Texture texture;
  texture.create(m_raytracer->width(), m_raytracer->height());
  m_running = true;
  while (m_running) {
    APP_PROFILE_SCOPE("MainLoop");

    sf::Event event;
    while (m_window->get_native_window()->pollEvent(event)) {
      APP_PROFILE_SCOPE("EventPolling");

      ImGui::SFML::ProcessEvent(event);

      if (event.type == sf::Event::Closed) {
        stop();
      }

      /*if (event.type == SDL_WINDOWEVENT &&
          event.window.windowID == SDL_GetWindowID(m_window->get_native_window())) {
        on_event(event.window);
      }*/
    }

    // Start the Dear ImGui frame
    ImGui::SFML::Update(*m_window->get_native_window(), deltaClock.restart());
    
    if (!m_minimized) {

      if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
          if (ImGui::MenuItem("Exit", "Cmd+Q")) {
            stop();
          }
          ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) {
          ImGui::MenuItem("Settings", nullptr, &m_show_some_panel);
          ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
      }

      // Scene settings widget ...
      if (m_show_some_panel) {
        ImGui::Begin("Scene settings", &m_show_some_panel);
        ImGui::Text("Hello World");
        static int scene_width = m_raytracer->width();
        ImGui::InputInt("Width", &scene_width);
        static int scene_height = m_raytracer->height();
        ImGui::InputInt("Height", &scene_height);
        static int scene_samples_per_pixel = m_raytracer->samplesPerPixel();
        ImGui::InputInt("Samples/pixel", &scene_samples_per_pixel);
        if(ImGui::Button("Render")) // https://github.com/ocornut/imgui/issues/2481
        {
          m_raytracer->setWidth(scene_width);
          m_raytracer->setHeight(scene_height);
          m_raytracer->setSamplesPerPixel(scene_samples_per_pixel);
          m_raytracer->trace(m_texture); 
        }
        ImGui::End();
      }

      // Display image
      {
        ImGui::SetNextWindowSize(ImVec2(0,0));  // https://github.com/ocornut/imgui/issues/2808
        ImGui::Begin("Scene");
        ImGui::Text("size = %d x %d", m_raytracer->width(), m_raytracer->height());
        static float zoom = 1.0;
        ImGui::SliderFloat("Zoom", &zoom, 0.25, 3.0, 0, 0);

        // Create SFML texture from the image data
        texture.update(/*reinterpret_cast<const sf::Uint8*>*/(m_texture.data()));

        // Display the texture 
        ImGui::Image(texture, ImVec2((float)m_raytracer->width()*zoom, (float)m_raytracer->height()*zoom));
        ImGui::End();
      }
    }

    // Rendering
    m_window->get_native_window()->clear();
    ImGui::SFML::Render(*m_window->get_native_window());
    m_window->get_native_window()->display();
  }

  return m_exit_status;
}

void App::Application::stop() {
  m_running = false;
}

/*void Application::on_event(const SDL_WindowEvent& event) {
  switch (event.event) {
    case SDL_WINDOWEVENT_CLOSE:
      return on_close();
    case SDL_WINDOWEVENT_MINIMIZED:
      return on_minimize();
    case SDL_WINDOWEVENT_SHOWN:
      return on_shown();
  }
}*/

void Application::on_minimize() {
  m_minimized = true;
}

void Application::on_shown() {
  m_minimized = false;
}

void Application::on_close() {
  stop();
}

}  // namespace App
