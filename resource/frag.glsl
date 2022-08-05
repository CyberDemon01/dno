#version 330 core

out vec4 color;
in vec2 tex_coords;

uniform sampler2D diffuse_texture;
uniform sampler2D specular_texture;
uniform sampler2D normal_texture;
uniform sampler2D height_texture;

void main() {
    color = texture(diffuse_texture, tex_coords);
}
