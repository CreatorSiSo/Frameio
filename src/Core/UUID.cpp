#include <random>

#include "Core/UUID.hpp"

namespace Frameio {

static std::random_device s_RandomDevice;
static std::mt19937_64 s_Engine64(s_RandomDevice());
static std::mt19937 s_Engine32(s_RandomDevice());
static std::uniform_int_distribution<uint64_t> s_UniformDistributionU64;
static std::uniform_int_distribution<uint32_t> s_UniformDistributionU32;

UUID::UUID() : m_UUID(s_UniformDistributionU64(s_Engine64)) {}

UUID::UUID(bool useInt32Range)
{
  // Generate a random uint32_t (0 to 4294967295)
  // then divide it by two to land in the positive range of int32_t (0 to 2147483647)
  m_UUID = s_UniformDistributionU32(s_Engine32) / 2;
}

UUID::UUID(uint64_t uuid) : m_UUID(uuid) {}

} // namespace Frameio
