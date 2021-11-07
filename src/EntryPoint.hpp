#pragma once

extern Framio::App *Framio::CreateApp();

int main(int argc, char **argv) {
  Framio::Log::Init();
  FR_CORE_INFO("Initialized Logger");
  FR_INFO("Initialized Logger");

  auto app = Framio::CreateApp();
  app->Run();
  delete app;
  exit(EXIT_SUCCESS);
}
