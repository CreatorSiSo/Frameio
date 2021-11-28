#include "frpch.hpp"

#include "WindowsWindow.hpp"

namespace Frameio {

static bool s_GLFWInitialized = false;

static void GLFWErrorCallbackFunction(int error, const char* description)
{
  FR_ERROR("{0} {1}", error, (std::string)description);
}

WindowsWindow::WindowsWindow(const WindowProps& props)
{
  Init(props);
}
WindowsWindow::~WindowsWindow()
{
  Shutdown();
}

Window* Window::Create(const WindowProps& props)
{
  return new WindowsWindow(props);
}

void WindowsWindow::Init(const WindowProps& props)
{
  m_Data.Title = props.Title;
  m_Data.Width = props.Width;
  m_Data.Height = props.Height;

  FR_CORE_INFO("Creating Window '{0}' ({1}, {2})", m_Data.Title, m_Data.Width, m_Data.Height);

  if (!s_GLFWInitialized) {
    // TODO: glfwTerminate on system shutdown
    int glfwInitialized = glfwInit();
    FR_ASSERT(glfwInitialized, "Failed to intialize GLFW!");
    glfwSetErrorCallback(GLFWErrorCallbackFunction);
    s_GLFWInitialized = true;
  }

  m_Window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);

  m_Context = new OpenGLContext(m_Window);
  m_Context->Init();

  glfwSetWindowUserPointer(m_Window, &m_Data);
  SetVSync(true);

  glfwSetWindowSizeCallback(m_Window,
                            [](GLFWwindow* window, int width, int height)
                            {
                              WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                              data.Width = width;
                              data.Height = height;

                              WindowResizeEvent e(width, height);
                              data.EventCallback(e);
                            });

  glfwSetWindowCloseCallback(m_Window,
                             [](GLFWwindow* window)
                             {
                               WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                               WindowCloseEvent event;
                               data.EventCallback(event);
                             });

  glfwSetKeyCallback(m_Window,
                     [](GLFWwindow* window, int key, int scancode, int action, int mods)
                     {
                       WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                       switch (action) {
                         case GLFW_PRESS: {
                           KeyDownEvent e(key, 0);
                           data.EventCallback(e);
                         } break;

                         case GLFW_RELEASE: {
                           KeyUpEvent e(key);
                           data.EventCallback(e);
                         } break;

                         case GLFW_REPEAT: {
                           KeyDownEvent e(key, 1);
                           data.EventCallback(e);
                         } break;
                       }
                     });

  glfwSetCharCallback(m_Window,
                      [](GLFWwindow* window, unsigned int keycode)
                      {
                        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                        KeyTypedEvent e(keycode);
                        data.EventCallback(e);
                      });

  glfwSetMouseButtonCallback(m_Window,
                             [](GLFWwindow* window, int button, int action, int mods)
                             {
                               WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                               switch (action) {
                                 case GLFW_PRESS: {
                                   MouseButtonDownEvent e(button);
                                   data.EventCallback(e);
                                 } break;

                                 case GLFW_RELEASE: {
                                   MouseButtonUpEvent e(button);
                                   data.EventCallback(e);
                                 } break;
                               }
                             });

  glfwSetScrollCallback(m_Window,
                        [](GLFWwindow* window, double xOffset, double yOffset)
                        {
                          WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                          MouseScrolledEvent e((float)xOffset, (float)yOffset);
                          data.EventCallback(e);
                        });

  glfwSetCursorPosCallback(m_Window,
                           [](GLFWwindow* window, double xPos, double yPos)
                           {
                             WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                             MouseMovedEvent e((float)xPos, (float)yPos);
                             data.EventCallback(e);
                           });
}

void WindowsWindow::Shutdown()
{
  glfwTerminate();
}

unsigned int WindowsWindow::GetWidth() const
{
  return m_Data.Width;
}
unsigned int WindowsWindow::GetHeight() const
{
  return m_Data.Height;
}

void WindowsWindow::OnUpdate()
{
  glfwPollEvents();
  m_Context->SwapBuffers();
}

void WindowsWindow::SetVSync(bool use)
{
  if (use)
    glfwSwapInterval(1);
  else
    glfwSwapInterval(0);

  m_Data.VSync = use;
}

bool WindowsWindow::IsVSync() const
{
  return m_Data.VSync;
}

} // namespace Frameio
