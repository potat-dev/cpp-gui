#include <GLFW/glfw3.h>

#include <iostream>

int main() {
  GLFWwindow* window;
  // int windowSizeW = 640, windowSizeH = 480;  // window size in pixels
  // initialize the library
  if (!glfwInit()) return -1;

  glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
  int count, windowWidth, windowHeight, monitorX, monitorY;

  // I am assuming that main monitor is in the 0 position
  GLFWmonitor** monitors = glfwGetMonitors(&count);
  const GLFWvidmode* videoMode = glfwGetVideoMode(monitors[0]);
  // width and height of the monitor
  windowWidth = static_cast<int>(videoMode->width);
  windowHeight = static_cast<int>(videoMode->height);

  glfwGetMonitorPos(monitors[0], &monitorX, &monitorY);

  // set the visibility window hint to false for subsequent window creation
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

  // create a windowed mode window and its OpenGL context
  window =
      glfwCreateWindow(windowWidth, windowHeight, "Hello World", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  // make the window's context current
  glfwMakeContextCurrent(window);

  // reset the window hints to default
  glfwDefaultWindowHints();

  glfwSetWindowPos(window, monitorX, monitorY);
  // monitorX + (videoMode->width - windowWidth) / 2,
  // monitorY + (videoMode->height - windowHeight) / 2);

  // show the window
  glfwShowWindow(window);

  // uncomment following line to see the border of window
  glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_FALSE);

  float fAngle = 0.0f;
  // Loop until the user closes the window
  while (!glfwWindowShouldClose(window)) {
    // render
    // glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    // glClear(GL_COLOR_BUFFER_BIT);
    // glLoadIdentity();
    // glRotatef(fAngle, 0, 0, 1);
    // glBegin(GL_QUADS);
    // glColor3f(0, 0, 1);
    // glVertex3f(-0.5, -0.5, -1);
    // glColor3f(0, 1, 0);
    // glVertex3f(0.5, -0.5, -1);
    // glColor3f(1, 0, 1);
    // glVertex3f(0.5, 0.5, -1);
    // glColor3f(1, 1, 0);
    // glVertex3f(-0.5, 0.5, -1);
    // glEnd();

    // rotating triangle
    // glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    // glClear(GL_COLOR_BUFFER_BIT);
    // glLoadIdentity();
    // glRotatef(fAngle, 0, 0, 1);
    // glBegin(GL_TRIANGLES);
    // glColor3f(1, 0, 0);
    // glVertex3f(-0.5, -0.5, -1);
    // glColor3f(0, 1, 0);
    // glVertex3f(0.5, -0.5, -1);
    // glColor3f(0, 0, 1);
    // glVertex3f(0, 0.5, -1);
    // glEnd();

    // four colored squares at the corners
    // glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    // glClear(GL_COLOR_BUFFER_BIT);
    // glLoadIdentity();
    // // do not rotate
    // // glRotatef(fAngle, 0, 0, 1);
    // glBegin(GL_QUADS);
    // glColor3f(1, 0, 0);
    // glVertex3f(-0.5, -0.5, -1);
    // glColor3f(0, 1, 0);
    // glVertex3f(0.5, -0.5, -1);
    // glColor3f(0, 0, 1);
    // glVertex3f(0.5, 0.5, -1);
    // glColor3f(1, 1, 0);
    // glVertex3f(-0.5, 0.5, -1);
    // glEnd();

    fAngle += 0.1;

    // swap front and back buffers
    glfwSwapBuffers(window);

    // poll for and process events
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}