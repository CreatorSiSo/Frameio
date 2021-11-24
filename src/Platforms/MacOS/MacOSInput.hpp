#pragma once

#include "frpch.hpp"

#include "Input/Input.hpp"

namespace Frameio {

class MacOSInput : public Input {
  protected:
    virtual bool IsKeyDownImpl(int keycode) override;
    virtual bool IsMouseButtonDownImpl(int button) override;
    virtual std::pair<float, float> GetMousePosImpl() override;
    virtual float GetMouseXImpl() override;
    virtual float GetMouseYImpl() override;
};

} // namespace Frameio
