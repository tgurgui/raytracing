/*
 * Copyright (c) 2022 Martin Helmut Fieber <info@martin-fieber.se>
 */

#include "Application.hpp"

#include <backends/imgui_impl_sdl.h>
#include <backends/imgui_impl_sdlrenderer.h>
#include <imgui.h>

#include "Core/Debug/Instrumentor.hpp"

namespace App {

Application::Application(const std::string& title) {
  APP_PROFILE_FUNCTION();

  unsigned int init_flags{SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER};
  if (SDL_Init(init_flags) != 0) {
    APP_ERROR("Error: %s\n", SDL_GetError());
    m_exit_status = ExitStatus::FAILURE;
  }

  m_window = std::make_unique<Window>(Window::Settings{title});
  m_raytracer = std::make_unique<Raytracer>(Raytracer{});
}

Application::~Application() {
  APP_PROFILE_FUNCTION();

  ImGui_ImplSDLRenderer_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  SDL_Quit();
}

ExitStatus App::Application::run() {
  APP_PROFILE_FUNCTION();

  if (m_exit_status == ExitStatus::FAILURE) {
    return m_exit_status;
  }

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io{ImGui::GetIO()};

  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable |
                    ImGuiConfigFlags_ViewportsEnable;

  // ImGUI font
  /*const float font_scaling_factor{m_window->get_scale()};
  const float font_size{18.0F * font_scaling_factor};
  io.Fonts->AddFontFromFileTTF("fonts/Manrope.ttf", font_size);
  io.FontDefault = io.Fonts->AddFontFromFileTTF("fonts/Manrope.ttf", font_size);
  io.FontGlobalScale = 1.0F / font_scaling_factor;*/

  // Setup Platform/Renderer backends
  ImGui_ImplSDL2_InitForSDLRenderer(m_window->get_native_window(), m_window->get_native_renderer());
  ImGui_ImplSDLRenderer_Init(m_window->get_native_renderer());

  m_running = true;
  while (m_running) {
    APP_PROFILE_SCOPE("MainLoop");

    SDL_Event event{};
    while (SDL_PollEvent(&event) == 1) {
      APP_PROFILE_SCOPE("EventPolling");

      ImGui_ImplSDL2_ProcessEvent(&event);

      if (event.type == SDL_QUIT) {
        stop();
      }

      if (event.type == SDL_WINDOWEVENT &&
          event.window.windowID == SDL_GetWindowID(m_window->get_native_window())) {
        on_event(event.window);
      }
    }

    // Start the Dear ImGui frame
    ImGui_ImplSDLRenderer_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    if (!m_minimized) {
      ImGui::DockSpaceOverViewport();

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
          m_texture = SDL_CreateTextureFromSurface(m_window->get_native_renderer(), m_raytracer->trace().GetSDLSurface());  //https://stackoverflow.com/questions/46155113/sdl2-draw-scene-to-texture-sdl2-rendertexture-like-sfml
        }
        ImGui::End();
      }

      // Display image
      if(m_texture)
      {
        ImGui::SetNextWindowSize(ImVec2(0,0));  // https://github.com/ocornut/imgui/issues/2808
        ImGui::Begin("Scene");
        ImGui::Text("size = %d x %d", m_raytracer->width(), m_raytracer->height());
        static float zoom = 1.0;
        ImGui::SliderFloat("Zoom", &zoom, 0.25, 3.0, 0, 0);
        ImGui::Image(m_texture, ImVec2((float)m_raytracer->width()*zoom, (float)m_raytracer->height()*zoom));
        ImGui::End();
      }
    }

    // Rendering
    ImGui::Render();

    SDL_SetRenderDrawColor(m_window->get_native_renderer(), 100, 100, 100, 255);
    SDL_RenderClear(m_window->get_native_renderer());
    ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderPresent(m_window->get_native_renderer());
  }

  return m_exit_status;
}

void App::Application::stop() {
  m_running = false;
}

void Application::on_event(const SDL_WindowEvent& event) {
  switch (event.event) {
    case SDL_WINDOWEVENT_CLOSE:
      return on_close();
    case SDL_WINDOWEVENT_MINIMIZED:
      return on_minimize();
    case SDL_WINDOWEVENT_SHOWN:
      return on_shown();
  }
}

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
