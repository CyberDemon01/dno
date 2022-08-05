#include "shader_prog.h"
#include "utils.h"

std::string shader_type_str(GLuint shader_type) {
  std::string result = {};
  switch (shader_type) {
  case GL_VERTEX_SHADER:
    result = "vertex shader";
    break;
  case GL_FRAGMENT_SHADER:
    result = "fragment shader";
    break;
  case GL_GEOMETRY_SHADER:
    result = "geometry shader";
    break;
  default:
    exit("unsdefine shader type");
    break;
  }
  return result;
}

shader_prog::shader_prog(const std::string& vpath, const std::string& fpath) {
  this->load(vpath, fpath);
}

shader_prog::~shader_prog() {
  glDeleteProgram(program);
}

void shader_prog::load(const std::string& vpath, const std::string& fpath) {
  auto vert_sh = this->compile_shader(vpath, GL_VERTEX_SHADER);
  auto frag_sh = this->compile_shader(fpath, GL_FRAGMENT_SHADER);

  program = glCreateProgram();

  glAttachShader(program, vert_sh);
  glAttachShader(program, frag_sh);
  glLinkProgram(program);

  glDeleteShader(vert_sh);
  glDeleteShader(frag_sh);

  GLint status = {};
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if (!status) {
    GLint info_len {};
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_len);
    std::string info(info_len + 1, 0);
    glGetProgramInfoLog(program, info_len, nullptr, info.data());
    exit("fail to link shader program\n{}", info);
  }
}

void shader_prog::bind() {
  glUseProgram(program);
}

void shader_prog::unbind() {
  glUseProgram(0);
}

void shader_prog::uniform(const std::string& name, const glm::mat3& m) {
  auto loc = glGetUniformLocation(program, name.c_str());
  if (loc == -1)
		exit("not fount uniform variable [uniform mat3 {0}]", name);
	glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(m));
}

void shader_prog::uniform(const std::string& name, const glm::mat4& m) {
  auto loc = glGetUniformLocation(program, name.c_str());
  if (loc == -1)
		exit("not fount uniform variable [uniform mat4 {0}]", name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m));
}

void shader_prog::uniform(const std::string& name, const glm::vec3& v) {
  auto loc = glGetUniformLocation(program, name.c_str());
  if (loc == -1)
		exit("not fount uniform variable [uniform vec3 {0}]", name);
	glUniform3fv(loc, 1, glm::value_ptr(v));
}

void shader_prog::uniform(const std::string& name, const glm::vec4& v) {
  auto loc = glGetUniformLocation(program, name.c_str());
  if (loc == -1)
		exit("not fount uniform variable [uniform vec4 {0}]", name);
	glUniform4fv(loc, 1, glm::value_ptr(v));
}

void shader_prog::uniform(const std::string& name, int i) {
  auto loc = glGetUniformLocation(program, name.c_str());
  if (loc == -1)
		exit("not fount uniform variable [uniform i {0}]", name);
	glUniform1i(loc, i);
}

void shader_prog::uniform(const std::string& name, float f) {
  auto loc = glGetUniformLocation(program, name.c_str());
  if (loc == -1)
		exit("not fount uniform variable [uniform f {0}]", name);
	glUniform1f(loc, f);
}

void shader_prog::uniform(const std::string& name, size_t ui) {
  auto loc = glGetUniformLocation(program, name.c_str());
  if (loc == -1)
		exit("not fount uniform variable [uniform ui {0}]", name);
	glUniform1ui(loc, ui);
}

GLuint shader_prog::compile_shader(const std::string& path, GLuint shader_type) {
  std::string text = read(path);
  std::string sst = shader_type_str(shader_type);
  auto src = text.c_str();
  auto id = glCreateShader(shader_type);
  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);
  GLint status {};
  glGetShaderiv(id, GL_COMPILE_STATUS, &status);
  if (!status) {
    GLint info_len {};
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &info_len);
    std::string info(info_len + 1, 0);
    glGetShaderInfoLog(id, info_len, nullptr, info.data());
    exit("fail to compile shader\n[{0}, {1}]\n{2}", path, sst, info);
  }
  return id;
}
