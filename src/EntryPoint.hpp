#pragma once

extern Frameio::App *Frameio::CreateApp();

int main(int argc, char **argv) {
  Frameio::Log::Init();
  FR_CORE_INFO("Initialized Logger");
  FR_INFO("Initialized Logger");

  auto app = Frameio::CreateApp();
  app->Run();
  delete app;
  exit(EXIT_SUCCESS);
}
