#include <glm/glm.hpp>
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_internal.h>

namespace ImGui {

bool DragFloat3(const char *label, glm::vec3 &value, float reset_value = 0.0f,
                float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f,
                const char *format = "%.3f", ImGuiSliderFlags flags = 0);

} // namespace ImGui
