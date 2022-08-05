#ifndef _utils_h
#define _utils_h

#include <string>
#include <spdlog/spdlog.h>
#include <GL/glew.h>
#include <stb_image.h>

template <typename ... Args>
void exit(const std::string& fmt, Args... args) {
  spdlog::error(fmt, args...);
  exit(EXIT_FAILURE);
}

template <typename ... Args>
void exit_if(bool value, const std::string& fmt, Args... args) {
  if (value) {
    spdlog::error(fmt, args...);
    exit(EXIT_FAILURE);
  }
}

std::string read(const std::string& path);

GLuint load_texture_from_file(const std::string& path);

#endif
