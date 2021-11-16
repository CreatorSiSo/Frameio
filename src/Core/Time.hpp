#pragma once

namespace Frameio {

class RealDeltaTime {
public:
  RealDeltaTime(float time = 0.0f) : m_Time(time) {}

  inline float GetSeconds() const { return m_Time; }
  inline float GetMilliseconds() const { return m_Time * 1000.0f; }

  operator float() const { return m_Time; }

private:
  float m_Time;
};

} // namespace Frameio
