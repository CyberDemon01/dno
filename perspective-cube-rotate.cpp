#include <bits/stdc++.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <spdlog/spdlog.h>
#include <vec2.hpp>
#include <mat4x4.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <trigonometric.hpp>
#include <stb_image.h>
#include "vec3.h"
#include "mat3.h"
#include "transform.h"

using namespace std;

#define printd(expr) cout << #expr " = " << (expr) << endl

void log_error_exit(const char* const fmt, ...) {
  va_list arg;
  va_start(arg, fmt);
  fprintf(stderr, "[\033[1;31merror\033[0m] ");
  vfprintf(stderr, fmt, arg);
  va_end(arg);
  fprintf(stderr, "\n");
  exit(EXIT_FAILURE);
}

#define resource "/home/soslan/Yandex.Disk/qt/dno/resource/"

class window {
  GLFWwindow* wnd = {};
  int         width = {};
  int         height = {};
  string      title = {};
public:
  window(string t, int w, int h) {
    title = t;
    width = w;
    height = h;
    this->init();
  }
  ~window() {
    glfwDestroyWindow(wnd);
    glfwTerminate();
  }
  bool run() const {
    return glfwWindowShouldClose(wnd) == 0;
  }
  void clear() const {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }
  void update() {
    glfwPollEvents();
    glfwSwapBuffers(wnd);
  }
  void backround(GLfloat red, GLfloat green, GLfloat blue) {
    glClearColor(red, green, blue, 1.0);
  }
  void show_info() {
    cout << "OpenGL " << glGetString(GL_VERSION) << endl;
    cout << "Render " << glGetString(GL_RENDERER) << endl;
  }
private:
  void init() {
    if (!glfwInit())
      log_error_exit("fail to init GLFW");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    wnd = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!wnd)
      log_error_exit("fail to create GLFW window");
    glfwMakeContextCurrent(wnd);
    glfwSetKeyCallback(wnd, key_callback);
  }
private:
  static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE and action == GLFW_RELEASE)
      glfwSetWindowShouldClose(window, true);
  }
private:
  window(const window&) = delete;
  void operator=(const window&) = delete;
};

auto read(string path) {
  ifstream input(path);
  if (!input.is_open())
    log_error_exit("fail to open that file [%s]", path.c_str());
  stringstream stream;
  stream << input.rdbuf();
  input.close();
  return stream.str();
}

struct shader_prog {
  GLuint program {};
public:
  shader_prog() = default;
  shader_prog(string vpath, string fpath) {
    this->load(vpath, fpath);
  }
  ~shader_prog() {
    glDeleteProgram(program);
  }
  void load(string vpath, string fpath) {
    auto vert_sh = this->compile_shader(resource + vpath, GL_VERTEX_SHADER);
    auto frag_sh = this->compile_shader(resource + fpath, GL_FRAGMENT_SHADER);

    program = glCreateProgram();

    glAttachShader(program, vert_sh);
    glAttachShader(program, frag_sh);
    glLinkProgram(program);

    glDeleteShader(vert_sh);
    glDeleteShader(frag_sh);

    GLint status {};
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (!status) {
      GLint info_len {};
      glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_len);
      string info(info_len + 1, 0);
      glGetProgramInfoLog(program, info_len, nullptr, &info[0]);
      cerr << "[ERROR] fail to link shader program:\n" << info << endl;
      log_error_exit("fail to link shader program");
    }
  }
  void use() {
    glUseProgram(program);
  }
  void unuse() {
    glUseProgram(0);
  }
  void uniform(const GLchar* name, const glm::vec4& v4) {
    auto loc = glGetUniformLocation(program, name);
    if (loc == -1)
      log_error_exit("not fount uniform variable [uniform vec4f %s]", name);
    glUniform4f(loc, v4.x, v4.y, v4.z, v4.w);
  }
  void uniform(const GLchar* name, const glm::vec3& v3) {
    auto loc = glGetUniformLocation(program, name);
    if (loc == -1)
      log_error_exit("not fount uniform variable [uniform vec4f %s]", name);
    glUniform3f(loc, v3.x, v3.y, v3.z);
  }
  void uniform(const GLchar* name, int v) {
    auto loc = glGetUniformLocation(program, name);
    if (loc == -1)
      log_error_exit("not fount uniform variable [uniform vec1i %s]", name);
    glUniform1i(loc, v);
  }
  void uniform(const GLchar* name, float v) {
    auto loc = glGetUniformLocation(program, name);
    if (loc == -1)
      log_error_exit("not fount uniform variable [uniform vec1f %s]", name);
    glUniform1f(loc, v);
  }
  void uniform(const GLchar* name, const mat3f& m) {
    auto loc = glGetUniformLocation(program, name);
    if (loc == -1)
      log_error_exit("not fount uniform variable [uniform vec4fv %s]", name);
    glUniformMatrix3fv(loc, 1, GL_FALSE, m.flat);
  }
  void uniform(const GLchar* name, const mat4f& m) {
    auto loc = glGetUniformLocation(program, name);
    if (loc == -1)
      log_error_exit("not fount uniform variable [uniform vec4fv %s]", name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, m.flat);
  }
private:
  GLuint compile_shader(string path, GLuint shader_type) {
    auto text = read(path);
    auto src = text.c_str();
    auto id = glCreateShader(shader_type);
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    GLint status {};
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);
    if (!status) {
      GLint info_len {};
      glGetShaderiv(id, GL_INFO_LOG_LENGTH, &info_len);
      string info(info_len + 1, 0);
      glGetShaderInfoLog(id, info_len, nullptr, &info[0]);
      log_error_exit("fail to compile shader [%s]:\n%s\n", path.c_str(), info.c_str());
    }
    return id;
  }
};

int main() {
  const int width  = 600;
  const int height = 600;
  window wnd("Open[GL]", width, height);
  wnd.show_info();

  if (glewInit() != GLEW_OK)
    log_error_exit("fail to init GLEW");

  glEnable(GL_DEPTH_TEST);

  shader_prog color_shader("vert.glsl", "frag.glsl");
  shader_prog texture_shader("vert.glsl", "frag_texture.glsl");

  GLfloat tringle_gradiend[] = {
    -0.9f, -0.9f, 0.0f, 1.0f, 1.0f, 0.0f,
    -0.5f, -0.1f, 0.0f, 0.0f, 1.0f, 1.0f,
    -0.1f, -0.9f, 0.0f, 1.0f, 0.0f, 1.0f,
  };

  GLfloat tringle_texture[] = {
     0.1f,  0.1f, 0.0f, 0.0f, 0.0f,
     0.5f,  0.9f, 0.0f, 0.5f, 1.0f,
     0.9f,  0.1f, 0.0f, 1.0f, 0.0f,
  };

  GLfloat rect_color[] = {
    0.1f, -0.9f, 0.0f, 0.2f, 0.4f, 0.8f,
    0.1f, -0.1f, 0.0f, 0.2f, 0.4f, 0.8f,
    0.9f, -0.1f, 0.0f, 0.2f, 0.4f, 0.8f,
    0.9f, -0.1f, 0.0f, 0.2f, 0.4f, 0.8f,
    0.9f, -0.9f, 0.0f, 0.2f, 0.4f, 0.8f,
    0.1f, -0.9f, 0.0f, 0.2f, 0.4f, 0.8f,
  };

  GLfloat rect_texture[] = {
    -0.9f, 0.1f, 0.0f, 0.0f, 0.0f,
    -0.9f, 0.9f, 0.0f, 0.0f, 1.0f,
    -0.1f, 0.9f, 0.0f, 1.0f, 1.0f,
    -0.1f, 0.1f, 0.0f, 1.0f, 0.0f,
  };

  GLuint rect_texture_indices[] = {
    0, 1, 2,
    0, 2, 3,
  };

  GLfloat cross[] = {
     0.0f,  1.0f,  0.0f, 1.0f, 1.0f, 1.0f,
     0.0f, -1.0f,  0.0f, 1.0f, 1.0f, 1.0f,
     1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f,
    -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f,
     0.0f,  0.0f, -1.0f, 1.0f, 1.0f, 1.0f,
     0.0f,  0.0f,  1.0f, 1.0f, 1.0f, 1.0f,
  };

  GLuint cross_incdices[] = {
    0, 1,
    2, 3,
    4, 5,
  };

  GLfloat cube[] = {
    -0.5f, -0.5f, -0.5f, 0.8f, 0.4f, 0.8f,
    -0.5f,  0.5f, -0.5f, 0.8f, 0.4f, 0.8f,
     0.5f,  0.5f, -0.5f, 0.8f, 0.4f, 0.8f,
     0.5f, -0.5f, -0.5f, 0.8f, 0.4f, 0.8f,
    -0.5f, -0.5f,  0.5f, 0.8f, 0.4f, 0.8f,
    -0.5f,  0.5f,  0.5f, 0.8f, 0.4f, 0.8f,
     0.5f,  0.5f,  0.5f, 0.8f, 0.4f, 0.8f,
     0.5f, -0.5f,  0.5f, 0.8f, 0.4f, 0.8f,
  };

  GLuint cube_indices[] = {
    0, 1, 2,
    0, 2, 3,
    2, 3, 6,
    3, 6, 7,
    4, 5, 7,
    5, 6, 7,
    0, 1, 4,
    1, 4, 5,
    0, 3, 4,
    3, 4, 7,
    1, 2, 5,
    2, 5, 6,
  };

  GLuint vao[6];
  GLuint vbo[6];
  GLuint ibo[3];

  glGenVertexArrays(size(vao), vao);
  glGenBuffers(size(vbo), vbo);
  glGenBuffers(size(ibo), ibo);

  // triangle gradiend
  glBindVertexArray(vao[0]);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(tringle_gradiend), tringle_gradiend, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(0));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // triangle texture
  glBindVertexArray(vao[1]);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(tringle_texture), tringle_texture, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(0));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // rect color
  glBindVertexArray(vao[2]);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(rect_color), rect_color, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(0));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // rect texture
  glBindVertexArray(vao[3]);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(rect_texture), rect_texture, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rect_texture_indices), rect_texture_indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(0));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);

  // cross
  glBindVertexArray(vao[4]);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cross), cross, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[1]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cross_incdices), cross_incdices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(0));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);

  // cube
  glBindVertexArray(vao[5]);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[2]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(0));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);

  stbi_set_flip_vertically_on_load(true);

  GLuint textures[2];
  glGenTextures(size(textures), textures);

  glBindTexture(GL_TEXTURE_2D, textures[0]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  int tex_width = {};
  int tex_height = {};
  int n_channels = {};
  auto img_buf = stbi_load("smile.jpg", &tex_width, &tex_height, &n_channels, 0);
  if (!img_buf)
    log_error_exit("fail load smile.jpg");
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_width, 0, GL_RGB, GL_UNSIGNED_BYTE, img_buf);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
  stbi_image_free(img_buf);

  glBindTexture(GL_TEXTURE_2D, textures[1]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  tex_width = {};
  tex_height = {};
  n_channels = {};
  img_buf = stbi_load("arch.jpg", &tex_width, &tex_height, &n_channels, 0);
  if (!img_buf)
    log_error_exit("fail load arch.jpg");
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_width, 0, GL_RGB, GL_UNSIGNED_BYTE, img_buf);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
  stbi_image_free(img_buf);

  mat4f projection = {};
  identity(projection);
  projection = perspective(radians(45.0f), float(width) / height, 0.1f, 100.0f);
  color_shader.use();
  color_shader.uniform("projection", projection);
  texture_shader.use();
  texture_shader.uniform("projection", projection);

  mat4f model = {};
  identity(model);

  mat4f view = {};
  identity(view);
  translate(view, {0.0f, 0.0f, 3.5f});
  color_shader.use();
  color_shader.uniform("view", view);
  texture_shader.use();
  texture_shader.uniform("view", view);

//  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  while (wnd.run()) {
    wnd.clear();
//    glViewport(0, 0, width, height);

    rotate(model, radians(1.0f), vec3f(0.3f, 1.0f, 0.3f));
    color_shader.use();
    color_shader.uniform("model", model);
    texture_shader.use();
    texture_shader.uniform("model", model);

    color_shader.use();
    glBindVertexArray(vao[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
    color_shader.unuse();

    texture_shader.use();
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glBindVertexArray(vao[1]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    texture_shader.unuse();

    color_shader.use();
    glBindVertexArray(vao[2]);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    color_shader.unuse();

    texture_shader.use();
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glBindVertexArray(vao[3]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    texture_shader.unuse();

    color_shader.use();
    glBindVertexArray(vao[4]);
    glDrawArrays(GL_LINES, 0, 6);
    glBindVertexArray(0);
    color_shader.unuse();

    color_shader.use();
    glBindVertexArray(vao[5]);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    color_shader.unuse();

    wnd.update();
  }
  glDeleteBuffers(size(vbo), vbo);
  glDeleteBuffers(size(ibo), ibo);
  glDeleteVertexArrays(size(vao), vao);
  glDeleteTextures(size(textures), textures);
  exit(0);
}
