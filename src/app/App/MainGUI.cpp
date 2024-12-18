/*
 * Copyright (c) 2022 Martin Helmut Fieber <info@martin-fieber.se>
 */

#include <exception>

#include "Gui/Application.hpp"
#include "Core/Debug/Instrumentor.hpp"
#include "Core/Log.hpp"

int main() {
  try {
    APP_PROFILE_BEGIN_SESSION_WITH_FILE("App", "profile.json");

    {
      APP_PROFILE_SCOPE("Test scope");
      App::Application app{"Raytracing"};
      app.run();
    }

    APP_PROFILE_END_SESSION();
  } catch (std::exception& e) {
    APP_ERROR("Main process terminated with: {}", e.what());
  }

  return 0;
}
