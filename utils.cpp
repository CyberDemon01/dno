#include <fstream>
#include <sstream>
#include "utils.h"

std::string read(const std::string &path) {
  std::ifstream input(path);
  if (!input.is_open())
    exit("fail to open that file {}", path);
  std::stringstream stream;
  stream << input.rdbuf();
  input.close();
  return stream.str();
}

GLuint channels_to_format(int n_channels) {
  GLuint format = {};
  switch (n_channels) {
  case 3: format = GL_RGB; break;
  case 4: format = GL_RGBA; break;
  }
  return format;
}

GLuint load_texture_from_file(const std::string& path) {
  GLuint tex = {};
  glGenTextures(1, &tex);

  glBindTexture(GL_TEXTURE_2D, tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int width, height, n_channels;
  auto tex_buf = stbi_load(path.c_str(), &width, &height, &n_channels, 0);
  if (!tex_buf)
    exit("{}: fail to load texture [{}]", __func__, path);
  auto format = channels_to_format(n_channels);
  if (!format)
    exit("{}: incorrect image format = {} [{}]", __func__, format, path);
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, tex_buf);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
  stbi_image_free(tex_buf);

  return tex;
}
