#pragma once

#include "frpch.hpp"

#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>

namespace Frameio {

class Log {
  public:
    static void Init();

    inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }

    inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

  private:
    static std::shared_ptr<spdlog::logger> s_CoreLogger;
    static std::shared_ptr<spdlog::logger> s_ClientLogger;
};

} // namespace Frameio

// Frameio Log macros
#define FR_CORE_TRACE(...) ::Frameio::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define FR_CORE_INFO(...) ::Frameio::Log::GetCoreLogger()->info(__VA_ARGS__)
#define FR_CORE_WARN(...) ::Frameio::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define FR_CORE_ERROR(...) ::Frameio::Log::GetCoreLogger()->error(__VA_ARGS__)
#define FR_CORE_FATAL(...) ::Frameio::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client Log macros
#define FR_TRACE(...) ::Frameio::Log::GetClientLogger()->trace(__VA_ARGS__)
#define FR_INFO(...) ::Frameio::Log::GetClientLogger()->info(__VA_ARGS__)
#define FR_WARN(...) ::Frameio::Log::GetClientLogger()->warn(__VA_ARGS__)
#define FR_ERROR(...) ::Frameio::Log::GetClientLogger()->error(__VA_ARGS__)
#define FR_FATAL(...) ::Frameio::Log::GetClientLogger()->critical(__VA_ARGS__)
