#pragma once

namespace Frameio {

constexpr bool Int32Range = true;

class UUID {
public:
  UUID();
  explicit UUID(bool useInt32Range);
  UUID(uint64_t uuid);
  UUID(const UUID&) = default;

  inline std::string ToString() const { return std::to_string(m_UUID); }

  operator uint64_t() const { return m_UUID; }

private:
  uint64_t m_UUID;
};

} // namespace Frameio

namespace std {

template <>
struct hash<Frameio::UUID> {
  size_t operator()(const Frameio::UUID& uuid) const noexcept { return hash<uint64_t>()((uint64_t)uuid); }
};

} // namespace std
