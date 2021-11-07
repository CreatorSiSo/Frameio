#include "frpch.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>

#include "Log.hpp"

namespace Frameio {

std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

void Log::Init() {
  spdlog::set_pattern("[%H:%M:%S] [Thread %t] %n %v");

  s_CoreLogger = spdlog::stdout_color_mt("[Core]");
  s_CoreLogger->set_level(spdlog::level::trace);

  s_ClientLogger = spdlog::stdout_color_mt("[App] ");
  s_ClientLogger->set_level(spdlog::level::trace);
}

} // namespace Frameio