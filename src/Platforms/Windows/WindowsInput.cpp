#include <GLFW/glfw3.h>

#include "Core/App.hpp"
#include "WindowsInput.hpp"

namespace Frameio {

Input *Input::s_Instance = new WindowsInput();

bool WindowsInput::IsKeyDownImpl(int keycode) {
  auto window =
      static_cast<GLFWwindow *>(App::GetApp().GetWindow().GetNativeWindow());
  auto state = glfwGetKey(window, keycode);
  return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool WindowsInput::IsMouseButtonDownImpl(int button) {
  auto window =
      static_cast<GLFWwindow *>(App::GetApp().GetWindow().GetNativeWindow());
  auto state = glfwGetMouseButton(window, button);
  return state == GLFW_PRESS;
}

std::pair<float, float> WindowsInput::GetMousePosImpl() {
  auto window =
      static_cast<GLFWwindow *>(App::GetApp().GetWindow().GetNativeWindow());
  double xPos, yPos;
  glfwGetCursorPos(window, &xPos, &yPos);
  return {(float)xPos, (float)yPos};
}

float WindowsInput::GetMouseXImpl() {
  auto [x, y] = GetMousePosImpl();
  return x;
}

float WindowsInput::GetMouseYImpl() {
  auto [x, y] = GetMousePosImpl();
  return y;
}

} // namespace Frameio
