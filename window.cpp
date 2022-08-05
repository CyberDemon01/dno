#include <iostream>
#include "window.h"
#include "utils.h"

window::window(const std::string& t, int w, int h) : title(t), width(w), height(h) {
  this->init();
}
window::~window() {
  glfwDestroyWindow(wnd);
  glfwTerminate();
}

bool window::is_close() const {
  return glfwWindowShouldClose(wnd);
}

void window::clear() const {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void window::update() {
  glfwPollEvents();
  glfwSwapBuffers(wnd);
}

bool window::poll_event(event& e) {
	e.type = event_type::none;
	if (poll.size()) {
		e = poll.front();
		poll.pop();
	}
	return e.type != event_type::none;
}

void window::info() const {
  spdlog::info("OpenGL {}", glGetString(GL_VERSION));
  spdlog::info("Render {}", glGetString(GL_RENDERER));
}

void window::init() {
  if (!glfwInit())
    exit("fail to init GLFW");
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  wnd = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  if (!wnd)
    exit("fail to create GLFW window");
  glfwMakeContextCurrent(wnd);
  glfwSetWindowUserPointer(wnd, this);
  glfwSetKeyCallback(wnd, key_callback);
  glfwSetCursorPosCallback(wnd, cursor_position_callback);
  glfwSetMouseButtonCallback(wnd, mouse_button_callback);
  glfwSetScrollCallback(wnd, scroll_callback);
}

void window::key_callback(GLFWwindow* wnd, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE and action == GLFW_RELEASE)
    glfwSetWindowShouldClose(wnd, true);
  auto self = reinterpret_cast<window*>(glfwGetWindowUserPointer(wnd));
	self->poll.push(event(keyboard_event(key, action)));
}

void window::cursor_position_callback(GLFWwindow* wnd, double xpos, double ypos) {
  auto self = reinterpret_cast<window*>(glfwGetWindowUserPointer(wnd));
	self->poll.push(event(mouse_event(mouse_position_event(xpos, ypos))));
}

void window::mouse_button_callback(GLFWwindow* wnd, int button, int action, int mods) {
  auto self = reinterpret_cast<window*>(glfwGetWindowUserPointer(wnd));
	self->poll.push(event(mouse_event(mouse_button_event(button, action))));
}

void window::scroll_callback(GLFWwindow* wnd, double xoffset, double yoffset) {
  auto self = reinterpret_cast<window*>(glfwGetWindowUserPointer(wnd));
  self->evt = event(mouse_event(mouse_wheel_event(xoffset, yoffset)));
}
