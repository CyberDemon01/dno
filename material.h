#ifndef _material_h
#define _material_h

#include <vector>
#include <unordered_set>
#include <string>
#include <GL/glew.h>
#include "shader_prog.h"
#include "utils.h"

enum class texture_type {diffuse, specular, normal, height};

struct texture {
  GLuint tex = {};
  texture_type type = {};
};

class mesh_material {
  std::vector<texture> textures = {};
public:
  mesh_material() = default;
  mesh_material(const std::vector<texture>& _textures);
  mesh_material(std::vector<texture>&& _textures);
  mesh_material(const mesh_material& _material);
  mesh_material(mesh_material&& _material);
  void set_units(shader_prog& s);
};

#endif
