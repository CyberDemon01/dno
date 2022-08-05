#ifndef _shader_prog_h
#define _shader_prog_h

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

struct shader_prog {
  GLuint program {};
public:
  shader_prog(const std::string& vpath, const std::string& fpath);
  ~shader_prog();
  void load(const std::string& vpath, const std::string& fpath);
  void bind();
  void unbind();
	void uniform(const std::string& name, const glm::mat3& m);
	void uniform(const std::string& name, const glm::mat4& m);
	void uniform(const std::string& name, const glm::vec3& v);
	void uniform(const std::string& name, const glm::vec4& v);
  void uniform(const std::string& name, int v);
  void uniform(const std::string& name, float v);
  void uniform(const std::string& name, size_t v);
private:
  GLuint compile_shader(const std::string& path, GLuint shader_type);
};

#endif
