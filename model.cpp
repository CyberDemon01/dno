#include "model.h"
#include "utils.h"

model_3d::model_3d(const std::string& _dir,
                   const std::string& _name) :
  dir(_dir), name(_name) {
  this->load();
}

void model_3d::paint(shader_prog& s) {
  for (auto& m : meshes)
    m.paint(s);
}

void model_3d::load() {
  std::string path = dir + name;
  Assimp::Importer importer = {};
  const auto scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);
  if (!scene|| scene->mFlags & AI_SCENE_FLAGS_ALLOW_SHARED || !scene->mRootNode)
    exit("assimp: fail to load model {0}\n{1}", path, importer.GetErrorString());
  process_node(scene->mRootNode, scene);
}

void model_3d::process_node(aiNode* node, const aiScene* scene) {
  for (size_t i = 0; i < node->mNumMeshes; i++) {
    auto ai_mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(process_mesh(ai_mesh, scene));
  }
  for (size_t i = 0; i < node->mNumChildren; i++)
    process_node(node->mChildren[i], scene);
}

mesh model_3d::process_mesh(aiMesh* ai_mesh, const aiScene* scene) {
  std::vector<vertex_attribs> vertices = {};
  std::vector<GLuint> indices = {};
  for (size_t i = 0; i < ai_mesh->mNumVertices; i++) {
    vec3f coords = {ai_mesh->mVertices[i].x,
                    ai_mesh->mVertices[i].y,
                    ai_mesh->mVertices[i].z};
    vec3f normal = {ai_mesh->mNormals[i].x,
                    ai_mesh->mNormals[i].y,
                    ai_mesh->mNormals[i].z};
    vec2f tex_coords = {};
    if (ai_mesh->mTextureCoords[0]) {
      tex_coords = {ai_mesh->mTextureCoords[0][i].x,
                    ai_mesh->mTextureCoords[0][i].y};
    }
    vertices.push_back({coords, normal, tex_coords});
  }
  for (size_t i = 0; i < ai_mesh->mNumFaces; i++) {
    aiFace face = ai_mesh->mFaces[i];
    for (size_t j = 0; j < face.mNumIndices; j++) {
      indices.push_back(face.mIndices[j]);
    }
  }
  auto ai_material = scene->mMaterials[ai_mesh->mMaterialIndex];
  std::vector<texture> textures = {};
  if (ai_material->GetTextureCount(aiTextureType_DIFFUSE)) {
    aiString str = {};
    ai_material->GetTexture(aiTextureType_DIFFUSE, 0, &str);
    spdlog::info("diffuse texture {0}", str.C_Str());
    textures.push_back({load_texture_from_file(dir + str.C_Str()), texture_type::diffuse});
  }
  if (ai_material->GetTextureCount(aiTextureType_SPECULAR)) {
    aiString str = {};
    ai_material->GetTexture(aiTextureType_SPECULAR, 0, &str);
    spdlog::info("specular texture {0}", str.C_Str());
    textures.push_back({load_texture_from_file(dir + str.C_Str()), texture_type::specular});
  }
  if (ai_material->GetTextureCount(aiTextureType_NORMALS)) {
    aiString str = {};
    ai_material->GetTexture(aiTextureType_NORMALS, 0, &str);
    spdlog::info("normal texture {0}", str.C_Str());
    textures.push_back({load_texture_from_file(dir + str.C_Str()), texture_type::normal});
  }
  if (ai_material->GetTextureCount(aiTextureType_HEIGHT)) {
    aiString str = {};
    ai_material->GetTexture(aiTextureType_HEIGHT, 0, &str);
    spdlog::info("normal texture {0}", str.C_Str());
    textures.push_back({load_texture_from_file(dir + str.C_Str()), texture_type::height});
  }
  spdlog::info("model load");
  return {vertices, indices, textures};
}
