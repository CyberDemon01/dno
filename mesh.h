#ifndef _mesh_h
#define _mesh_h

#include <string>
#include <vector>
#include <GL/glew.h>
#include "shader_prog.h"
#include "vec2.h"
#include "vec3.h"
#include "material.h"

struct vertex_attribs {
  vec3f coords = {};
  vec3f normal = {};
  vec2f tex_coords = {};
};

class mesh {
public:
  std::vector<vertex_attribs> vertices = {};
  std::vector<GLuint> indices = {};
private:
  GLuint vao = {};
  GLuint vbo = {};
  GLuint ebo = {};
  mesh_material material = {};
public:
  mesh(const std::vector<vertex_attribs>& _vertices,
       const std::vector<GLuint>& _indices,
       const mesh_material& _material);
  mesh(std::vector<vertex_attribs>&& _vertices,
       std::vector<GLuint>&& _indices,
       mesh_material&& _material);
  mesh(const mesh& m);
  mesh(mesh&& m);
  ~mesh();
  void paint(shader_prog& s);
private:
  void setup();
};

#endif
