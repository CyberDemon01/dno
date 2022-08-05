#include "material.h"

mesh_material::mesh_material(const std::vector<texture>& _textures) :
  textures(_textures) {}

mesh_material::mesh_material(std::vector<texture>&& _textures) :
  textures(std::move(_textures)) {}

mesh_material::mesh_material(const mesh_material& _material) :
  textures(_material.textures) {}

mesh_material::mesh_material(mesh_material&& _material) :
  textures(std::move(_material.textures)) {}

void mesh_material::set_units(shader_prog& s) {
  exit_if(textures.size() > GL_MAX_TEXTURE_UNITS,
          "textures per shader > GL_MAX_TEXTURE_UNITS");
  for (size_t i = 0; i < textures.size(); i++) {
    if (textures[i].type != texture_type::diffuse)
      continue;
    glActiveTexture(GL_TEXTURE0 + i);
    std::string name = {};
    switch (textures[i].type) {
    case texture_type::diffuse: name = "diffuse_texture"; break;
    case texture_type::specular: name = "specular_texture"; break;
    case texture_type::normal: name = "normal_texture"; break;
    case texture_type::height: name = "height_texture"; break;
    }
    s.uniform(name, i);
    glBindTexture(GL_TEXTURE_2D, textures[i].tex);
  }
}
