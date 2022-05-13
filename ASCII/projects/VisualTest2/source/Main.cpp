/*
 * Copywrite 2022 Dodge Lafnitzegger
 */

#include "GLFW/glfw3.h"

int main(void) {
  if (!glfwInit()) {
    return 1;
  }

  GLFWwindow * window0 = glfwCreateWindow(640, 360, glfwGetMonitorName(glfwGetPrimaryMonitor()), nullptr, nullptr);
  GLFWwindow * window1 = glfwCreateWindow(640, 360, "Test", nullptr, nullptr);

  while (true) {
    glfwWaitEvents();
  }

  glfwTerminate();
  return 0;
}

