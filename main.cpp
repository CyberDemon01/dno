#include <bits/stdc++.h>
#include <vec2.hpp>
#include <mat4x4.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <trigonometric.hpp>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "window.h"
#include "shader_prog.h"
#include "utils.h"
#include "model.h"

using namespace std;

#define resource "/home/ueba/Yandex.Disk/qt/dno/resource/"
#define paths(path) resource + std::string(path)

int main() {
  const int width  = 800;
  const int height = 800;
  window wnd("Open[GL]", width, height);
  wnd.info();

  if (glewInit() != GLEW_OK)
    exit("fail to init GLEW");

  glEnable(GL_DEPTH_TEST);

  shader_prog texture_shader(paths("vert.glsl"), paths("frag.glsl"));

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), float(width) / height, 0.1f, 100.0f);

	texture_shader.bind();
	texture_shader.uniform("projection", projection);

	glm::vec3 camera_pos = {0.0f, 0.0f, 4.0f};
	glm::vec3 camera_front = {0.0f, 0.0f, -1.0f};
	glm::vec3 camera_up = {0.0f, 1.0f, 0.0f};
	const float camera_speed = 0.1f;
	const float mouse_sens = 0.1f;
	float lastx = {};
	float lasty = {};
	float xpos = {};
	float ypos = {};
	float yaw = -90.0f;
	float pitch = {};
	bool motion = false;

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	model_3d asuka("./Asuka/", "Asuka.obj");

  event evt = {};
  while (!wnd.is_close()) {
    wnd.clear();
		while (wnd.poll_event(evt)) {
      switch(evt.type) {
      case event_type::none:
        break;
      case event_type::keyboard:
        switch (evt.keyboard.key) {
				case GLFW_KEY_D:
					camera_pos += glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
          break;
				case GLFW_KEY_A:
					camera_pos -= glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
          break;
				case GLFW_KEY_W:
					camera_pos += camera_front * camera_speed;
					break;
				case GLFW_KEY_S:
					camera_pos -= camera_front * camera_speed;
          break;
        }
        break;
      case event_type::mouse:
        switch (evt.mouse.type) {
        case mouse_event_type::position:
					xpos = evt.mouse.position.xpos;
					ypos = evt.mouse.position.ypos;
					if (motion) {
						yaw += (evt.mouse.position.xpos - lastx) * mouse_sens;
						pitch += (lasty - evt.mouse.position.ypos) * mouse_sens;
						lastx = evt.mouse.position.xpos;
						lasty = evt.mouse.position.ypos;
					}
          break;
        case mouse_event_type::button:
					if (evt.mouse.button.action == GLFW_PRESS) {
						lastx = xpos;
						lasty = ypos;
						motion = true;
					} else {
						motion = false;
					}
          break;
        case mouse_event_type::wheel:
          break;
        }
        break;
      }
    }
		camera_front = glm::normalize(glm::vec3(cos(glm::radians(yaw)) * cos(glm::radians(pitch)), sin(glm::radians(pitch)), sin(glm::radians(yaw)) * cos(glm::radians(pitch))));
		view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);
		texture_shader.bind();
		texture_shader.uniform("view", view);
		model = glm::rotate(model, glm::radians(0.5f), glm::vec3{0.0f, 0.5f, 0.0f});
		texture_shader.bind();
		texture_shader.uniform("model", model);
		asuka.paint(texture_shader);
    wnd.update();
  }
  exit(0);
}
