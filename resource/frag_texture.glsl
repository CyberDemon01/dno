#version 330 core

out vec4 color;
in vec2 tex_coords;
uniform sampler2D user_texture;

void main() {
    color = texture(user_texture, tex_coords);
}
