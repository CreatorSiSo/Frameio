#pragma once

#include "frpch.hpp"

#include "Core/Time.hpp"
#include "Events/Event.hpp"

namespace Frameio {

class Layer {
  public:
    Layer(const std::string& debugName = "Layer");
    virtual ~Layer() = default;

    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void OnUpdate(RealDeltaTime realDeltaTime) {}
    virtual void OnImGuiRender() {}
    virtual void OnEvent(Event& e) {}

    inline const std::string& GetName() const { return m_DebugName; }

  private:
    std::string m_DebugName;
};

} // namespace Frameio
