#ifndef _window_h
#define _window_h

#include <string>
#include <queue>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "event.h"

class window {
  std::string       title = {};
  int               width = {};
  int               height = {};
  GLFWwindow*       wnd = {};
  event             evt = {};
	std::queue<event> poll = {};
public:
  window(const std::string& _title, int _width, int _height);
  ~window();
  bool is_close() const;
  void clear() const;
  void update();
  bool poll_event(event& e);
  void info() const;
private:
  void init();
private:
  static void key_callback(GLFWwindow* wnd, int key, int scancode, int action, int mods);
  static void cursor_position_callback(GLFWwindow* wnd, double xpos, double ypos);
  static void mouse_button_callback(GLFWwindow* wnd, int button, int action, int mods);
  static void scroll_callback(GLFWwindow* wnd, double xoffset, double yoffset);
private:
  window(const window&) = delete;
  void operator=(const window&) = delete;
};

#endif
