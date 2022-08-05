#ifndef _model_h
#define _model_h

#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "mesh.h"
#include "material.h"
#include "shader_prog.h"

class model_3d {
  std::vector<mesh> meshes = {};
  std::string dir = {};
  std::string name = {};
public:
  model_3d(const std::string& _dir, const std::string& _name);
  void paint(shader_prog& s);
private:
  void load();
  void process_node(aiNode* node, const aiScene* scene);
  mesh process_mesh(aiMesh* ai_mesh, const aiScene* scene);
};

#endif
