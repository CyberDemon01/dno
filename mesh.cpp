#include "mesh.h"

mesh::mesh(const std::vector<vertex_attribs>& _vertices,
           const std::vector<GLuint>& _indices,
           const mesh_material& _material) :
  vertices(_vertices),
  indices(_indices),
  material(_material) {
  this->setup();
}

mesh::mesh(std::vector<vertex_attribs>&& _vertices,
           std::vector<GLuint>&& _indices,
           mesh_material&& _material) :
  vertices(std::move(_vertices)),
  indices(std::move(_indices)),
  material(std::move(_material)) {
  this->setup();
}

mesh::mesh(const mesh& m) :
  vertices(m.vertices),
  indices(m.indices),
  material(m.material) {
  this->setup();
}

mesh::mesh(mesh&& m) :
  vertices(std::move(m.vertices)),
  indices(std::move(m.indices)),
  vao(m.vao),
  vbo(m.vbo),
  ebo(m.ebo),
  material(std::move(m.material)) {
  m.vao = -1;
  m.vbo = -1;
  m.ebo = -1;
}

mesh::~mesh() {
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);
}

void mesh::paint(shader_prog& s) {
  s.bind();
  glBindVertexArray(vao);
  material.set_units(s);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
  s.unbind();
}

void mesh::setup() {
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex_attribs), vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, sizeof(vertex_attribs), (void*)(offsetof(vertex_attribs, coords)));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_attribs), (void*)(offsetof(vertex_attribs, normal)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_attribs), (void*)(offsetof(vertex_attribs, tex_coords)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
}
