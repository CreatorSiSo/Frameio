#pragma once

#include "frpch.hpp"

#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>

namespace Framio {

class Log {
public:
  static void Init();

  inline static std::shared_ptr<spdlog::logger> &GetCoreLogger() {
    return s_CoreLogger;
  }

  inline static std::shared_ptr<spdlog::logger> &GetClientLogger() {
    return s_ClientLogger;
  }

private:
  static std::shared_ptr<spdlog::logger> s_CoreLogger;
  static std::shared_ptr<spdlog::logger> s_ClientLogger;
};

} // namespace Framio

// Framio Log macros
#define FR_CORE_TRACE(...) ::Framio::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define FR_CORE_INFO(...) ::Framio::Log::GetCoreLogger()->info(__VA_ARGS__)
#define FR_CORE_WARN(...) ::Framio::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define FR_CORE_ERROR(...) ::Framio::Log::GetCoreLogger()->error(__VA_ARGS__)
#define FR_CORE_FATAL(...) ::Framio::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client Log macros
#define FR_TRACE(...) ::Framio::Log::GetClientLogger()->trace(__VA_ARGS__)
#define FR_INFO(...) ::Framio::Log::GetClientLogger()->info(__VA_ARGS__)
#define FR_WARN(...) ::Framio::Log::GetClientLogger()->warn(__VA_ARGS__)
#define FR_ERROR(...) ::Framio::Log::GetClientLogger()->error(__VA_ARGS__)
#define FR_FATAL(...) ::Framio::Log::GetClientLogger()->fatal(__VA_ARGS__)
