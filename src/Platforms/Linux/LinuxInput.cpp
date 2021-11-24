#include <GLFW/glfw3.h>

#include "Core/App.hpp"
#include "LinuxInput.hpp"

namespace Frameio {

Input* Input::s_Instance = new LinuxInput();

bool LinuxInput::IsKeyDownImpl(int keycode)
{
  auto window = static_cast<GLFWwindow*>(App::GetApp().GetWindow().GetNativeWindow());
  auto state = glfwGetKey(window, keycode);
  return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool LinuxInput::IsMouseButtonDownImpl(int button)
{
  auto window = static_cast<GLFWwindow*>(App::GetApp().GetWindow().GetNativeWindow());
  auto state = glfwGetMouseButton(window, button);
  return state == GLFW_PRESS;
}

std::pair<float, float> LinuxInput::GetMousePosImpl()
{
  auto window = static_cast<GLFWwindow*>(App::GetApp().GetWindow().GetNativeWindow());
  double xPos, yPos;
  glfwGetCursorPos(window, &xPos, &yPos);
  return { (float)xPos, (float)yPos };
}

float LinuxInput::GetMouseXImpl()
{
  auto [x, y] = GetMousePosImpl();
  return x;
}

float LinuxInput::GetMouseYImpl()
{
  auto [x, y] = GetMousePosImpl();
  return y;
}

} // namespace Frameio
